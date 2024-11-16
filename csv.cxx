#include <algorithm>
#include <vector>
#include <string>
#include <stdexcept>
#include <cctype>

namespace csv
{

class Parser
{
private:
    std::vector<std::string> m_title_fields{};
    using record_type = std::vector<std::string>;
    std::vector<record_type> m_data{};

    struct string_trim_result
    { std::string::const_iterator begin, end; };

    static string_trim_result string_trim(std::string::const_iterator string_begin, std::string::const_iterator string_end);
    static std::vector<std::string> parse_line(std::string::const_iterator line_begin, std::string::const_iterator line_end);
public:
    Parser(std::string const& title_line);
    void add_records(std::string const& data_str);
    std::vector<std::string> const& titles() const;
    record_type const& record_at(size_t index) const;
    std::string const& field_at(size_t record_index, size_t field_index) const;
    size_t record_count() const;
    size_t field_count() const;
};

Parser::string_trim_result Parser::string_trim(std::string::const_iterator string_begin, std::string::const_iterator string_end)
{
    using Iterator = std::string::const_iterator;
    Iterator new_begin = string_begin, new_last = std::prev(string_end);
    while (new_begin != string_end && !std::isgraph(*new_begin))
        ++new_begin;
    while (new_last != new_begin && !std::isgraph(*new_last))
        --new_last;
    return {new_begin, std::next(new_last)};
}

std::vector<std::string> Parser::parse_line(std::string::const_iterator line_begin, std::string::const_iterator line_end)
{
    using Iterator = std::string::const_iterator;
    auto const trimmed = string_trim(line_begin, line_end);
    // Each field is enclosed in double quotes
    std::vector<std::string> fields;
    Iterator field_begin = trimmed.begin, field_end;
    do {
        field_begin = std::find(field_begin, trimmed.end, '\"');
        field_end = std::find(std::next(field_begin), trimmed.end, '\"');
        if (field_begin == trimmed.end)
            break; // No more fields
        if (field_begin != trimmed.end && field_end == trimmed.end)
            throw std::runtime_error("Invalid CSV format: missing quote");
        fields.push_back(std::string(std::next(field_begin), field_end));
        field_begin = std::next(field_end);
    } while (true);
    return fields;
}

Parser::Parser(std::string const& title_line)
    : m_title_fields(parse_line(title_line.cbegin(), title_line.cend())), m_data{}
{}

void Parser::add_records(std::string const& data_str)
{
    using Iterator = std::string::const_iterator;
    // Trim the data string
    auto const trimmed = string_trim(data_str.cbegin(), data_str.cend());
    // Parse the title line
    Iterator record_begin = trimmed.begin, record_end = std::find(record_begin, trimmed.end, '\n');
    auto const title_fields = parse_line(record_begin, record_end);
    if (title_fields != this->titles())
        throw std::runtime_error("Invalid CSV format: title line mismatch");
    // Parse the data lines
    while (record_end != trimmed.end) {
        record_begin = std::next(record_end);
        record_end = std::find(record_begin, trimmed.end, '\n');
        auto const fields = parse_line(record_begin, record_end);
        if (fields.size() != this->field_count())
            throw std::runtime_error("Invalid CSV format: field count mismatch");
        m_data.push_back(fields);
    }
}

std::vector<std::string> const& Parser::titles() const
{ return m_title_fields; }

Parser::record_type const& Parser::record_at(size_t index) const
{ return m_data.at(index); }

std::string const& Parser::field_at(size_t record_index, size_t field_index) const
{ return m_data.at(record_index).at(field_index); }

size_t Parser::record_count() const
{ return m_data.size(); }

size_t Parser::field_count() const
{ return m_title_fields.size(); }

} // namespace csv

#include <fstream>

namespace file
{

class FileBuffer
{
private:
    std::string m_filename;
    std::string m_buffer;
public:
    FileBuffer(std::string const& filename);
    std::string const& filename() const;
    std::string const& buffer_ref() const;
    std::string&& buffer_release();
};

FileBuffer::FileBuffer(std::string const& filename)
    : m_filename(filename), m_buffer{}
{
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file)
        throw std::runtime_error("Failed to open file: " + filename);
    m_buffer.resize(file.tellg());
    file.seekg(0);
    file.read(&m_buffer[0], m_buffer.size());
}

std::string const& FileBuffer::filename() const
{ return m_filename; }

std::string const& FileBuffer::buffer_ref() const
{ return m_buffer; }

std::string&& FileBuffer::buffer_release()
{ return std::move(m_buffer); }

} // namespace file

namespace mail
{

// Corresponding to the CSV file format
class PostalAddress
{
protected:
    std::string m_country
    std::string m_postal_code;
    std::string m_location;
public:
    // Something else...
    // Should be done by `@Owen`
};

// Other information
class UserInfo
{
protected:
    std::string m_name;
    std::string m_email;
    std::string m_phone_number;
public:
    // Something else...
    // Should be done by `@Joe`
};

// Pure storage struct, no need for any method
struct Dimension
{
    long double length;
    long double width;
    long double height;
};

class PackageInfo
{
protected:
    Dimension m_dimension;
    long double m_weight;
public:
    // Something else...
    // Should be done by `@Joe`
};

class ShipmentInfo
{
protected:
    PostalAddress m_origin;
    PostalAddress m_destination;
    PackageInfo m_package;
    UserInfo m_sender;
    std::string m_shipment_id;
    std::string m_shipment_date;
    std::string m_service_type;
    long double m_cost;
public:
    // Something else...
    // Should be done by `@Joe`
};

void interface()
{
    // Something else...
    // Should be done by `@Joe`
}

} // namespace mail

#include <iostream>

extern "C" int csv_test()
{
    try {
        file::FileBuffer meta_buffer("test.csv.fmt");
        file::FileBuffer data_buffer("test.csv");
        csv::Parser parser(meta_buffer.buffer_ref());
        parser.add_records(data_buffer.buffer_ref());
        for (auto const& title : parser.titles())
            std::cout << '\"' << title  << '\"' << ' ';
        std::cout << '\n';
        for (size_t i = 0; i < parser.record_count(); ++i) {
            for (size_t j = 0; j < parser.field_count(); ++j)
                std::cout << '\"' << parser.field_at(i, j) << '\"' << ' ';
            std::cout << '\n';
        }
    } catch (std::exception const& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}

int main()
{
    return csv_test();
}
