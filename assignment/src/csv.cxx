#include "csv.hxx"

#include <algorithm>
#include <cctype>
#include <format>

namespace csv
{

namespace Impl
{

static auto string_strip(std::string_view str) -> std::string_view
{
    auto const first = std::ranges::find_if(str, std::isgraph);
    auto const last = std::ranges::find_last_if(first, str.cend(), std::isgraph);
    return last != str.cend() ? std::string_view{first, last + 1} : std::string_view{};
}

/*!
 * @brief Parse a line of csv-sheet.
 * @param line The line of csv-sheet.
 * @return The fields of the csv-sheet.
 * @throw `std::logic_error` If the format of the line is invalid.
 */
static auto parse_line(std::string_view line) -> std::vector<std::string_view>
{
    line = string_strip(line);
    if (line.empty()) {
        throw std::logic_error("Invalid csv format: empty line");
    }
    std::vector<std::string_view> fields;
    for (
        // The content of each field is of the range [field_begin + 1, field_end)
        // `field_begin` is the iterator to the first '\"' of the field
        // `field_end` is the iterator to the last '\"' of the field + 1
        std::string_view::const_iterator field_begin, field_end = line.cbegin();
        field_end != line.cend();
        ++field_end
    ) {
        field_begin = std::ranges::find(field_end, line.cend(), '\"');
        if (field_begin == line.cend()) {
            throw std::logic_error("Invalid csv format: missing '\"'");
        }
        field_end = std::ranges::find(field_begin + 1, line.cend(), '\"');
        if (field_end == line.cend()) {
            throw std::logic_error("Invalid csv format: missing '\"'");
        }

        fields.emplace_back(field_begin + 1, field_end);
    }
    return fields;
}

} // namespace Impl

Parser::Parser(std::string_view meta, std::vector<std::string_view> data_str_refs)
{
    // Parse the metadata
    {
        // Strip the leading and trailing whitespaces
        meta = Impl::string_strip(meta);

        // Name of each field
        std::string_view::const_iterator begin_of_line = meta.cbegin();
        std::string_view::const_iterator end_of_line = std::ranges::find(begin_of_line, meta.cend(), '\n');
        std::string_view const meta_first_line{begin_of_line, end_of_line};
        auto const parsed_field_names = Impl::parse_line(meta_first_line);
        for (auto const& field_name : parsed_field_names) {
            this->m_field_names.emplace_back(field_name);
        }

        // Type of each field
        begin_of_line = end_of_line + 1;
        end_of_line = std::ranges::find(begin_of_line, meta.cend(), '\n');
        std::string_view const meta_second_line{begin_of_line, end_of_line};
        auto const parsed_field_types = Impl::parse_line(meta_second_line);
        for (auto const& field_type : parsed_field_types) {
            auto const type_iter = data_type_mapping.find(field_type);
            if (type_iter == data_type_mapping.cend()) {
                throw std::out_of_range("Invalid data type");
            }
            this->m_field_types.emplace_back(type_iter->second);
        }

        // Check if the number of field names and field types are the same
        if (this->m_field_names.size() != this->m_field_types.size()) {
            throw std::out_of_range("The number of field names and field types are not the same");
        }
    }

    // Parse the data
    // The cost of copying the string_view is negligible
    for (std::string_view data_str_ref : data_str_refs) {
        static std::size_t count_of_ref = 0;
        // Strip the leading and trailing whitespaces
        data_str_ref = Impl::string_strip(data_str_ref);
        
        // The first line must be the field names and same as the metadata
        std::string_view::const_iterator begin_of_line = data_str_ref.cbegin();
        std::string_view::const_iterator end_of_line = std::ranges::find(end_of_line, data_str_ref.cend(), '\n');
        std::string_view const data_field_name_line{begin_of_line, end_of_line};
        auto const parsed_field_names = Impl::parse_line(data_field_name_line);
        if (parsed_field_names.size() != this->m_field_names.size()) {
            throw std::out_of_range(std::format(
                "In data_str_refs[{}]:"
                "  The number of field names is not the same as the metadata"
                "  Expected: {}, Actual: {}",
                count_of_ref, this->m_field_names.size(), parsed_field_names.size()
            ));
        }
        for (std::size_t i = 0; i < parsed_field_names.size(); ++i) {
            if (parsed_field_names[i] != this->m_field_names[i]) {
                throw std::out_of_range(std::format(
                    "In data_str_refs[{}]:"
                    "  The field names are not the same as the metadata"
                    "  Expected: {}, Actual: {}",
                    count_of_ref, this->m_field_names[i], parsed_field_names[i]
                ));
            }
        }

        // The rest of lines are records of the csv-sheet
        do {
            begin_of_line = end_of_line + 1;
            end_of_line = std::ranges::find(begin_of_line, data_str_ref.cend(), '\n');
            std::string_view const data_record_line{begin_of_line, end_of_line};
            auto const parsed_record = Impl::parse_line(data_record_line);
            if (parsed_record.size() != this->m_field_names.size()) {
                throw std::out_of_range(std::format(
                    "In data_str_refs[{}]:"
                    "  The number of fields is not the same as the metadata"
                    "  Expected: {}, Actual: {}",
                    count_of_ref, this->m_field_names.size(), parsed_record.size()
                ));
            }
        }
    }
}

auto Parser::at(std::size_t row, std::size_t col) const -> valid_data_type
{

}

} // namespace csv
