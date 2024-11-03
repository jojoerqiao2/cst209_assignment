#include "csv.hxx"

#include <algorithm>
#include <cctype>
#include <format>
#include <charconv>

namespace csv
{

namespace Impl
{

static auto 
string_strip(
    std::string_view str
) noexcept
-> std::string_view
{
    std::string_view::const_iterator begin = str.cbegin(), end = str.cend();
    while (begin != end && !std::isgraph(*begin))
        ++begin;
    while (end != begin && !std::isgraph(*(end - 1)))
        --end;
    return std::string_view{begin, end};
}

static auto 
parse_line(
    std::string_view line
) THROW_EXCEPTION(csv::InvalidFormat)
-> std::vector<std::string_view>
{
    line = string_strip(line);
    if (line.empty()) {
        throw csv::InvalidFormat("Empty line");
    }
    std::vector<std::string_view> fields;
    for (
        std::string_view::const_iterator field_begin, field_end = line.cbegin();
        field_end != line.cend();
        ++field_end
    ) {
        field_begin = std::ranges::find(field_end, line.cend(), '\"');
        if (field_begin == line.cend()) {
            throw csv::InvalidFormat("Missing '\"'");
        }
        field_end = std::ranges::find(field_begin + 1, line.cend(), '\"');
        if (field_end == line.cend()) {
            throw csv::InvalidFormat("Missing '\"'");
        }

        fields.emplace_back(field_begin + 1, field_end);
    }
    return fields;
}

} // namespace Impl

#define mapping(type) \
    { "${" #type "}", typeid(type) }
const std::unordered_map<std::string_view, std::type_index> Type::data_type_mapping {
    mapping(integer),
    mapping(string),
};
#undef mapping

InvalidFormat::InvalidFormat(std::string_view message) noexcept
    : std::logic_error{ message.empty() ? std::string{base_message} : std::format("{}: {}", base_message, message) }
{}
auto InvalidFormat::what() const noexcept -> const char*
{ return this->std::logic_error::what(); }

UnsupportedType::UnsupportedType(std::string_view message) noexcept
    : std::out_of_range{ message.empty() ? std::string{base_message} : std::format("{}: {}", base_message, message) }
{}
auto UnsupportedType::what() const noexcept -> const char*
{ return this->std::out_of_range::what(); }

FieldMismatch::FieldMismatch(std::string_view message) noexcept
    : std::out_of_range{ message.empty() ? std::string{base_message} : std::format("{}: {}", base_message, message) }
{}
auto FieldMismatch::what() const noexcept -> const char*
{ return this->std::out_of_range::what(); }

ParsingError::ParsingError(std::string_view message) noexcept
    : std::runtime_error{ message.empty() ? std::string{base_message} : std::format("{}: {}", base_message, message) }
{}
auto ParsingError::what() const noexcept -> const char*
{ return this->std::runtime_error::what(); }

Parser::Parser(
    std::string_view meta, 
    std::vector<std::string_view> data_refs
) THROW_EXCEPTION(csv::InvalidFormat, csv::UnsupportedType, csv::FieldMismatch)
    : m_title_names{}, m_field_types{}, m_records{}
{
    {
        meta = Impl::string_strip(meta);

        std::string_view::const_iterator begin_of_line = meta.cbegin();
        std::string_view::const_iterator end_of_line = std::ranges::find(begin_of_line, meta.cend(), '\n');
        std::string_view const title_names{begin_of_line, end_of_line};
        auto const parsed_title_names = Impl::parse_line(title_names);
        for (auto const& field_name : parsed_title_names) {
            this->m_title_names.emplace_back(field_name);
        }

        begin_of_line = end_of_line + 1;
        end_of_line = std::ranges::find(begin_of_line, meta.cend(), '\n');
        std::string_view const field_types{begin_of_line, end_of_line};
        auto const parsed_field_types = Impl::parse_line(field_types);
        for (auto const& field_type : parsed_field_types) {
            auto const type_iter = data_type_mapping.find(field_type);
            if (type_iter == data_type_mapping.cend()) {
                throw csv::UnsupportedType{field_type};
            }
            this->m_field_types.emplace_back(type_iter->second);
        }

        if (this->m_title_names.size() != this->m_field_types.size()) {
            throw csv::FieldMismatch{std::format(
                "Expect same number of field names and field types, but got {} and {}.",
                this->m_title_names.size(), this->m_field_types.size()
            )};
        }
    }

    for (std::string_view data_ref : data_refs) {
        this->append_data(data_ref);
    }
}

void 
Parser::append_data(
    std::string_view data_ref
) THROW_EXCEPTION(csv::FieldMismatch)
{
    data_ref = Impl::string_strip(data_ref);
    
    std::string_view::const_iterator begin_of_line = data_ref.cbegin();
    std::string_view::const_iterator end_of_line = std::ranges::find(begin_of_line, data_ref.cend(), '\n');
    std::string_view const title_line{begin_of_line, end_of_line};
    auto const parsed_field_names = Impl::parse_line(title_line);
    if (parsed_field_names.size() != this->field_count()) {
        throw csv::FieldMismatch{std::format(
            "Expect the number of field names is {}, but got {} in the title line.",
            this->field_count(), parsed_field_names.size()
        )};
    }
    for (std::size_t i = 0; i < parsed_field_names.size(); ++i) {
        if (parsed_field_names[i] != this->m_title_names[i]) {
            throw csv::FieldMismatch{std::format(
                "Except the field name \"{}\", but got \"{}\".",
                this->m_title_names[i], parsed_field_names[i]
            )};
        }
    }

    for (size_t line = 1; end_of_line != data_ref.cend(); ++line) {
        begin_of_line = end_of_line + 1;
        end_of_line = std::ranges::find(begin_of_line, data_ref.cend(), '\n');
        std::string_view const record_line{begin_of_line, end_of_line};
        auto const parsed_record = Impl::parse_line(record_line);
        // Check if the number of fields inside this record is the same as the metadata
        if (parsed_record.size() != this->field_count()) {
            throw csv::FieldMismatch{std::format(
                "In line {}, expect the number of fields is {}, but got {}.",
                line, this->field_count(), parsed_record.size()
            )};
        }
        this->m_records.emplace_back(parsed_record);
    }
}

auto 
Parser::parse_record_at(
    std::size_t row, 
    std::size_t col
) const THROW_EXCEPTION(csv::ParsingError, std::logic_error, std::out_of_range)
-> Type::parsed_data_type
{
    const auto& field_str_ref = this->m_records.at(row).at(col);
    const auto& field_type = this->m_field_types.at(col);
    if (field_type == typeid(integer)) {
        integer value;
        auto const end_ptr = field_str_ref.data() + field_str_ref.size();
        auto const begin_ptr = field_str_ref.data();
        auto const [ptr, ec] = std::from_chars(begin_ptr, end_ptr, value);
        if (ec == std::errc::result_out_of_range) {
            throw csv::ParsingError{
                std::format("The field \"{}\" is out of range", field_str_ref)
            };
        }
        if (ec == std::errc::invalid_argument || ptr != end_ptr) {
            throw csv::ParsingError{
                std::format("Failed to parse the field \"{}\" as integer", field_str_ref)
            };
        }
        return value;
    }
    if (field_type == typeid(string)) {
        return std::string{field_str_ref};
    }
    // This should not happen
    throw std::logic_error("Unknown data type and should not happen");
}

auto 
Parser::parse_record_at(
    std::size_t row, 
    std::string_view field_name
) const THROW_EXCEPTION(csv::FieldMismatch, csv::ParsingError, std::logic_error, std::out_of_range)
-> Type::parsed_data_type
{
    auto const field_iter = std::ranges::find(this->m_title_names, field_name);
    if (field_iter == this->m_title_names.cend()) {
        throw csv::FieldMismatch{
            std::format("The field name \"{}\" is not found", field_name)
        };
    }
    auto const col = std::distance(this->m_title_names.cbegin(), field_iter);
    return this->parse_record_at(row, col);
}

} // namespace csv
