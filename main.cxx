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
    std::string m_country;
    std::string m_postal_code;
    std::string m_location;
public:
    PostalAddress() = default;
    PostalAddress(std::string country, std::string postal_code, std::string location) {
        m_country = country;
        m_postal_code = postal_code;
        m_location = location;
    }

    std::string getCountry() const {
        return m_country;
    }

    void setCountry(const std::string& country) {
        m_country = country;
    }

    std::string getPostalCode() const {
        return m_postal_code;
    }

    void setPostalCode(const std::string& postal_code) {
        m_postal_code = postal_code;
    }

    std::string getLocation() const {
        return m_location;
    }

    void setLocation(const std::string& location) {
        m_location = location;
    }
    void display() {
        std::cout << "Country: " << m_country << std::endl;
        std::cout << "Postal code: " << m_postal_code << std::endl;
        std::cout << "location: " << m_location << std::endl;
    }
};

// Other information
class UserInfo
{
protected:
    std::string m_name;
    std::string m_email;
    std::string m_phone_number;
public:
    UserInfo() = default;
    UserInfo(std::string name, std::string email, std::string phone_number) {
        std::string m_name = name;
        std::string m_email = email;
        std::string m_phone_number = phone_number;
    }
    std::string getName() const {
        return m_name;
    }

    void setName(const std::string& name) {
        m_name = name;
    }

    std::string getEmail() const {
        return m_email;
    }

    void set(const std::string& email) {
        m_email = email;
    }

    std::string getPhoneNumber() const {
        return m_phone_number;
    }

    void setPhoneNumber(const std::string& phone_number) {
        m_phone_number = phone_number;
    }
    void display() {
        std::cout << "Name: " << m_name << std::endl;
        std::cout << "Email: " << m_email << std::endl;
        std::cout << "Phone number: " << m_phone_number << std::endl;
    }
    
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
    PackageInfo() = default;
    PackageInfo(long double length, long double width, long double height, long double weight) {
        m_dimension.length = length;
        m_dimension.width = width;
        m_dimension.height = height;
        m_weight = weight;
    }

    long double getLength() const {
        return m_dimension.length;
    }

    void setLength(long double length) {
        m_dimension.length = length;
    }

    long double getWidth() const {
        return m_dimension.width;
    }

    
    void setWidth(long double width) {
        m_dimension.width = width;
    }

    
    long double getHeight() const {
        return m_dimension.height;
    }

    
    void setHeight(long double height) {
        m_dimension.height = height;
    }

    
    long double getWeight() const {
        return m_weight;
    }

    
    void setWeight(long double weight) {
        m_weight = weight;
    }
    void display() {
        std::cout << "Weight:" << m_weight << std::endl;
        std::cout << "length: " << m_dimension.length << std::endl;
        std::cout << "Width: " << m_dimension.width << std::endl;
        std::cout << "Height " << m_dimension.height << std::endl;
    }
};

class ShipmentInfo
{
protected:
    UserInfo m_user;
    PostalAddress m_origin;
    PostalAddress m_destination;
    PackageInfo m_package;
    UserInfo m_sender;
    std::string m_shipment_id;
    std::string m_shipment_date;
    std::string m_service_type;
    long double m_cost;
public:
    ShipmentInfo() = default;
    ShipmentInfo(const PostalAddress& origin, const PostalAddress& destination,
        const PackageInfo& package, const UserInfo& user,const UserInfo& sender,
        const std::string& shipment_id, const std::string& shipment_date,
        const std::string& service_type)
    {
        m_origin = origin;
        m_destination = destination;
        m_package = package;
        m_user = user;
        m_sender = sender;
        m_shipment_id = shipment_id;
        m_shipment_date = shipment_date;
        m_service_type = service_type; 
       
    }

    // Getters
    PostalAddress getOrigin() const { return m_origin; }
    PostalAddress getDestination() const { return m_destination; }
    PackageInfo getPackage() const { return m_package; }
    UserInfo getUser() const { return m_user; }
    UserInfo getSender() const { return m_sender; }
    std::string getShipmentId() const { return m_shipment_id; }
    std::string getShipmentDate() const { return m_shipment_date; }
    std::string getServiceType() const { return m_service_type; }
    long double getCost() const { return m_cost; }

    // Setters
    void setOrigin(const PostalAddress& origin) { 
        m_origin = origin;
    }
    void setDestination(const PostalAddress& destination) {
        m_destination = destination;
    }
    void setPackage(const PackageInfo& package) { 
        m_package = package; 
    }
    void setSender(const UserInfo& user) {
        m_sender = user;
    }
    void setSender(const UserInfo& sender) {
        m_sender = sender; 
    }
    void setShipmentId(const std::string& shipment_id) {
        m_shipment_id = shipment_id;
    }
    void setShipmentDate(const std::string& shipment_date) {
        m_shipment_date = shipment_date; 
    }
    void setServiceType(const std::string& service_type) { 
        m_service_type = service_type; 
    }
    void setCost(long double cost) { 
        m_cost = cost; 
    }
    void display() {
        std::cout << "Account" << std::endl;
        m_user.display();
        std::cout << std::endl;
        std::cout << "Origin" << std::endl;
        m_origin.display();
        std::cout << std::endl;
        std::cout << "Destination" << std::endl;
        m_destination.display();
        std::cout << std::endl;
        std::cout << "Sender information" << std::endl;
        m_sender.display();
        std::cout << std::endl;
        std::cout << "Shipment information" << std::endl;
        m_package.display();
        std::cout << "Shipmentid: " << m_shipment_id << std::endl;
        std::cout << "shipmentdate: " << m_shipment_date << std::endl;
        std::cout << std::endl;
        std::cout << " service type: " << m_service_type << std::endl;
        std::cout << std::endl;
        std::cout << "Cost: " << m_cost << std::endl;
    }
};


void interface()
{
    std::cout << "********************* Ship now *********************" << std::endl;
    std::cout << "*******1.Please enter your account******************" << std::endl;
    std::cout << "****** 2.Enter your departure and destination ******" << std::endl;
    std::cout << "****** 3.Enter your service type *******************" << std::endl;
    std::cout << "****** 4.Describe your shipment ********************" << std::endl;
    std::cout << "****** 5.Get shipping prices ***********************" << std::endl;
    std::cout << "****** 6.Continue to book online *******************" << std::endl;
    
    
    std::string username, useremail, usernumber, ocountry, opostalcode, olocation,
        sendername, senderemail, sendernumber, dcountry, dpostalcode, dlocation,
        shipmentid, shipmentdate, servicetype;
    long double length, width, height, weight, cost;
    char  confirm;
    int i = 1;
    while (i) {
        std::cout << "Account" << std::endl;
        std::cout << "please enter your account:( name | email | phone number ) " << std::endl;
        std::cin >> username;
        std::cin >> useremail;
        std::cin >> usernumber;
        UserInfo user(username, useremail, usernumber);
        std::cout << "Login successful!" << std::endl;
        std::cout << std::endl;

        std::cout << "Origin and Destination" << std::endl;
        std::cout << "Please enter your origin: ( country | postal code | location ) ";
        std::cin >> ocountry;
        std::cin >> opostalcode;
        std::cin >> olocation;
        PostalAddress origin(ocountry, opostalcode, olocation);
        std::cout << "Please enter your destination: ( country | postal code | location ) ";
        std::cin >> dcountry;
        std::cin >> dpostalcode;
        std::cin >> dlocation;
        std::cout << "Please enter the sender information: ( name | email | phone number )" << std::endl;
        std::cin >> sendername;
        std::cin >> senderemail;
        std::cin >> sendernumber;
        UserInfo sender(sendername, senderemail, sendernumber);
        PostalAddress destination(dcountry, dpostalcode, dlocation);
        std::cout << std::endl;

        std::cout << "ServiceType" << std::endl;
        std::cout << "Please enter your serviceType: ( Sea transport | Air transport | Land transport) ";
        std::cin >> servicetype;
        std::cout << std::endl;

        std::cout << "Package Description" << std::endl;
        std::cout << "Please enter your shipmentid" << std::endl;
        std::cin >> shipmentid;
        std::cout << "Please enter your shipmentdate" << std::endl;
        std::cin >> shipmentdate;
        std::cout << "Please enter your weight(kg): " << std::endl;
        std::cin >> weight;
        std::cout << "Please enter your length(cm): " << std::endl;
        std::cin >> length;
        std::cout << "Please enter your width(cm): " << std::endl;
        std::cin >> width;
        std::cout << "Please enter your height(cm): " << std::endl;
        std::cin >> height;
        PackageInfo package(length, width, height, weight);
        ShipmentInfo u1(origin, destination, package, user, sender, shipmentid, shipmentdate, servicetype);
        std::cout << std::endl;
        
        //need calculate Fees

        u1.setCost(cost);
        std::cout << "The shipping fee is: " << u1.getCost() << std::endl;
        std::cout << std::endl;
       
        std::cout << "Details are as follows" << std::endl;
        u1.display();
        //Confirm whether to pay
        std::cout << "Do you want to confirm the shipment? (y/n): ";
        std::cin >> confirm;
        if (confirm == 'y' || confirm == 'Y') {
            std::cout << "Shipment confirmed. Please proceed to payment." << std::endl;
            std::cout << "Payment successful. Thank you!" << std::endl;
        }
        else {
            std::cout << "Shipment not confirmed." << std::endl;
        }

        //Use again or exit
        std::cout << "Do you want to continue booking online? (y/n): " << std::endl;
        std::cin >> confirm;
        if (confirm == 'n' || confirm == 'N') {
            std::cout << "Thank you for your use !" << std::endl;
            i--;

        }

    }

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
