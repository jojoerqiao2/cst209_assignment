#pragma once

#ifndef CSV_PARSER_HXX
#define CSV_PARSER_HXX

#include <vector>
#include <string>
#include <unordered_map>
#include <string_view>
#include <array>
#include <variant>

#include <cstdint>
#include <typeindex>

#include <stdexcept>

#include "utility.hxx"

namespace csv
{

class Type
{
public:
    using invalid = ::std::monostate;
    using string = ::std::string;
    using integer = ::std::int64_t;
    using parsed_data_type = ::std::variant<invalid, string, integer>;
protected:
    static const std::unordered_map<std::string_view, std::type_index> data_type_mapping;
};

class Error
{
public:
    virtual auto what() const noexcept -> const char* = 0;
    virtual ~Error() = default;
};

class InvalidFormat
    : public std::logic_error
    , public Error
{
protected:
    static constexpr std::string_view base_message = "Invalid CSV format";
public:
    InvalidFormat(std::string_view message = {}) noexcept;
    virtual auto what() const noexcept -> const char* override;
    virtual ~InvalidFormat() = default;
};

class UnsupportedType
    : public std::out_of_range
    , public Error
{
protected:
    static constexpr std::string_view base_message = "Unsupported data type";
public:
    UnsupportedType(std::string_view message = {}) noexcept;
    virtual auto what() const noexcept -> const char* override;
    virtual ~UnsupportedType() = default;
};

class FieldMismatch
    : public std::out_of_range
    , public Error
{
protected:
    static constexpr std::string_view base_message = "Field mismatch";
public:
    FieldMismatch(std::string_view message = {}) noexcept;
    virtual auto what() const noexcept -> const char* override;
    virtual ~FieldMismatch() = default;
};

class ParsingError
    : public std::runtime_error
    , public Error
{
protected:
    static constexpr std::string_view base_message = "Parsing error";
public:
    ParsingError(std::string_view message = {}) noexcept;
    virtual auto what() const noexcept -> const char* override;
    virtual ~ParsingError() = default;
};

class Parser
    : public Type
{
private:
    using record_type = std::vector<std::string_view>;
    // Metadata of each csv-sheet
    std::vector<std::string> m_title_names;
    std::vector<std::type_index> m_field_types;

    // Records of each csv-sheet
    std::vector<record_type> m_records;
public:
    Parser(
        std::string_view meta_ref, 
        std::vector<std::string_view> data_refs = {}
    ) THROW_EXCEPTION(csv::InvalidFormat, csv::UnsupportedType, csv::FieldMismatch);

    void 
    append_data(
        std::string_view data_ref
    ) THROW_EXCEPTION(csv::FieldMismatch);

    void clear_records() noexcept
    { this->m_records.clear(); }

    auto field_names() const noexcept -> decltype(auto)
    { return (this->m_title_names); }

    auto record_count() const noexcept -> std::size_t
    { return this->m_records.size(); }

    auto field_count() const noexcept -> std::size_t
    { return this->m_title_names.size(); }

    auto empty() const noexcept -> bool
    { return this->m_records.empty(); }

    auto 
    operator[](
        std::array<std::size_t, 2> const& index
    ) const THROW_EXCEPTION(std::out_of_range)
    -> std::string
    { return std::string{ this->m_records.at(index[0]).at(index[1]) }; }

    auto 
    parse_record_at(
        std::size_t row, 
        std::size_t col
    ) const THROW_EXCEPTION(csv::ParsingError, std::logic_error, std::out_of_range)
    -> Type::parsed_data_type;

    auto 
    parse_record_at(
        std::size_t row, 
        std::string_view field_name
    ) const THROW_EXCEPTION(csv::FieldMismatch, csv::ParsingError, std::logic_error, std::out_of_range)
    -> Type::parsed_data_type;
};

}

#endif // CSV_PARSER_HXX
