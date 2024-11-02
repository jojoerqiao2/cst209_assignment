#pragma once

#ifndef CSV_PARSER_HXX
#define CSV_PARSER_HXX

#include <vector>
#include <string>
#include <unordered_map>
#include <string_view>
#include <span>
#include <variant>

#include <cstdint>
#include <typeindex>

#include "config.hxx"

namespace csv
{

/*!
 * The `Parser` requires a more restricted and modified format of csv:
 * 1. The first row is assumed as the titles of all of fields;
 * 2. All fields must be quoted (that is, enclosed within double-quote characters);
 * 3. Each field must be singled-lined;
 * 4. Double-quote is not allow in each field;
 * 5. The leading and trailing whitespaces of each field are stripped;
 */
class Parser
{
public:
    using string = ::std::string;
    using integer = ::std::int64_t;
    using valid_data_type = std::variant<string, integer>;

private:
    using namespace std::literals;
    // The valid data types for the CSV parser. In this project, we only support integer and string.
    static inline const std::unordered_map<std::string_view, std::type_index> data_type_mapping = {
        { "string"sv, std::type_index(typeid(string)) },
        { "integer"sv, std::type_index(typeid(integer)) },
    };

    // Metadata of each csv-sheet
    std::vector<std::string> m_field_names;
    std::vector<std::type_index> m_field_types;

    // The `Parser` does not own the data of the csv-sheet.
    std::vector<std::vector<std::string_view>> m_field_str_refs;
public:
    Parser() = default;
    /*!
     * @param meta The metadata of the csv-sheet.
     * Example of metadata file:
     * ```
     * "postcode", "country", "location"
     * "${integer}", "${string}", "${string}"
     * ```
     * This will be parsed as:
     * ```
     * ---------------------------
     * |postcode|country|location|
     * |integer |string | string |
     * ---------------------------
     * ```
     * @param data_str_refs The data of the csv-sheet.
     * Example of data file:
     * ```
     * "postcode", "country", "location"
     * "1000", "Australia", "Sydney"
     * ```
     * The first line must be the field names and same as the metadata.
     * If the field names are not the same as the metadata, the program will throw an exception.
     * The rest of lines are records of the csv-sheet.
     * The type of each field is expected to be the same as the metadata.
     * However, the program will not check the type of each field at this moment.
     */
    Parser(std::string_view meta, std::vector<std::string_view> data_str_refs);

    auto get_field_names() const -> std::span<const std::string>
    { return std::span{this->m_field_names}; }

    // Get the data of the csv-sheet. If the data type is not the same as the metadata, the program will throw an exception.
    auto at(std::size_t row, std::size_t col) const -> valid_data_type;
    auto at(std::size_t row, std::string_view field_name) const -> valid_data_type;
};

}

#endif // CSV_PARSER_HXX
