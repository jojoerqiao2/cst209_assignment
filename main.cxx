#include <bits/stdc++.h>

// Lecturer said that we need `using namespace std;`. Make her happy.
using namespace std;

namespace csv
{

class Parser
{
private:
    vector<string> m_title_fields;
    typedef vector<string> record_type;
    vector<record_type> m_data;

    struct string_trim_result
    { string::const_iterator begin, end; };

    static string_trim_result string_trim(string::const_iterator string_begin, string::const_iterator string_end);
    static record_type parse_line(string::const_iterator line_begin, string::const_iterator line_end);
public:
    Parser(string const& title_line)
        : m_title_fields(parse_line(title_line.begin(), title_line.end()))
        , m_data()
    {}
    void add_records( string const& data_str);
    vector<string> const& titles() const { return m_title_fields; }
    record_type const& record_at(size_t index) const { return m_data.at(index); }
    string const& field_at(size_t record_index, size_t field_index) const { return record_at(record_index).at(field_index); }
    size_t record_count() const { return m_data.size(); }
    size_t field_count() const { return m_title_fields.size(); }
};

Parser::string_trim_result 
Parser::string_trim(
    string::const_iterator string_begin, 
    string::const_iterator string_end
)
{
    typedef string::const_iterator Iterator;
    Iterator new_begin = string_begin, new_last = string_end - 1;
    while (new_begin != string_end && ! isgraph(*new_begin))
        ++new_begin;
    while (new_last != new_begin && ! isgraph(*new_last))
        --new_last;
    Parser::string_trim_result result;
    result.begin = new_begin;
    result.end = new_last + 1;
    return result;
}

Parser::record_type
Parser::parse_line(
    string::const_iterator line_begin, 
    string::const_iterator line_end
)
{
    typedef string::const_iterator Iterator;
    Parser::string_trim_result const trimmed = string_trim(line_begin, line_end);
    // Each field is enclosed in double quotes
    Parser::record_type fields;
    Iterator field_begin = trimmed.begin, field_end;
    do {
        field_begin =  find(field_begin, trimmed.end, '\"');
        field_end =  find(field_begin + 1, trimmed.end, '\"');
        if (field_begin == trimmed.end)
            break; // No more fields
        if (field_begin != trimmed.end && field_end == trimmed.end)
            throw runtime_error("Invalid CSV format: missing quote");
        fields.push_back(string(field_begin + 1, field_end));
        field_begin = field_end + 1;
    } while (true);
    return fields;
}

void 
Parser::add_records(
    string const& data_str
)
{
    typedef string::const_iterator Iterator;
    // Trim the data string
    Parser::string_trim_result const trimmed = string_trim(data_str.begin(), data_str.end());
    // Parse the title line
    Iterator record_begin = trimmed.begin;
    Iterator record_end = find(record_begin, trimmed.end, '\n');
    Parser::record_type const title_fields = parse_line(record_begin, record_end);
    if (title_fields != this->titles())
        throw runtime_error("Invalid CSV format: title line mismatch");
    // Parse the data lines
    while (record_end != trimmed.end) {
        record_begin = record_end + 1;
        record_end =  find(record_begin, trimmed.end, '\n');
        Parser::record_type const fields = parse_line(record_begin, record_end);
        if (fields.size() != this->field_count())
            throw runtime_error("Invalid CSV format: field count mismatch");
        m_data.push_back(fields);
    }
}

} // namespace csv

namespace mail
{

class Location
{
public:
    virtual ~Location() = 0;
    bool operator<(Location const& other) const { return (string)(*this) < (string)(other); }
    virtual operator string() const = 0;
    virtual string to_string() const = 0;
};
Location::~Location() = default;
class FromLocation
    : public Location
{
protected:
    string city;
public:
    FromLocation(string const& city) 
        : city(city)
    {}
    virtual ~FromLocation() = default;

    virtual operator string() const override { return "{ " + this->city + " }"; }
    virtual string to_string() const override { return "From: { " + this->city + " }"; }
};
class ToLocation
    : public Location
{
protected:
    string city;
public:
    ToLocation(string const& city) 
        : city(city) 
    {}
    virtual ~ToLocation() = default;

    virtual operator string() const override { return "{ " + this->city + " }"; }
    virtual string to_string() const override { return "To: { " + this->city + " }"; }
};

typedef pair<FromLocation, ToLocation> Route;
Route
make_route(
    FromLocation from, 
    ToLocation to
)
{ return make_pair(from, to); }

string const distance_map_filename = "distance.csv";
class RouteToDistance
{
public:
    typedef Route RouteType;
    typedef unsigned int DistanceType;
protected:
    static map<RouteType, DistanceType> distance_map_init(string const& distance_map_file);
    static map<RouteType, DistanceType> const distance_map;
public:
    bool exists(RouteType const& route) const
    { return distance_map.find(route) != distance_map.end(); }
    DistanceType operator()(RouteType const& route) const
    { 
        try { return distance_map.at(route); }
        catch (out_of_range const&) { throw out_of_range("Route not found"); }
    }
} const route_to_distance;

map<RouteToDistance::RouteType, RouteToDistance::DistanceType> 
RouteToDistance::distance_map_init(
    string const& distance_map_filename
)
{
    map<RouteToDistance::RouteType, RouteToDistance::DistanceType> distance_map;
    ifstream distance_map_stream(distance_map_filename.c_str());
    if (!distance_map_stream)
        throw runtime_error("Failed to open distance map file");
    
    string distance_map_title_line;
    getline(distance_map_stream, distance_map_title_line);
    
    distance_map_stream.seekg(0);
    string file_buffer((istreambuf_iterator<char>(distance_map_stream)), istreambuf_iterator<char>());

    csv::Parser distance_map_parser(distance_map_title_line);
    distance_map_parser.add_records(file_buffer);

    for (size_t i = 0; i < distance_map_parser.record_count(); ++i) {
        string const& from_city = distance_map_parser.field_at(i, 0);
        string const& to_city = distance_map_parser.field_at(i, 1);
        RouteToDistance::RouteType const route = make_pair(FromLocation(from_city), ToLocation(to_city));
        istringstream ss(distance_map_parser.field_at(i, 2));
        RouteToDistance::DistanceType distance;
        ss >> distance;
        distance_map[route] = distance;
    }

    return distance_map;
}

map<RouteToDistance::RouteType, RouteToDistance::DistanceType> const RouteToDistance::distance_map = RouteToDistance::distance_map_init(distance_map_filename);

class Centimeter
{
public:
    typedef double ValueType;
private:
    ValueType value;
public:
    Centimeter(ValueType value)
        : value(value > 0 ? value : -1)
    {
        if (this->value == -1)
            throw runtime_error("Invalid centimeter value");
    }
    static string unit() { return "cm"; }
    operator ValueType() const { return this->value; }
};

class Freight
{
public:
    virtual ~Freight() = 0;

    virtual double volumetric_weight(Centimeter l, Centimeter w, Centimeter h, unsigned int packages) const = 0;
};
Freight::~Freight() = default;
class AirFreight
    : public Freight
{
public:
    virtual ~AirFreight() = default;

    virtual double volumetric_weight(Centimeter l, Centimeter w, Centimeter h, unsigned int packages) const override
    { return l * w * h / 6000.0 * packages; }
} const air_freight;
class OceanFreight
    : public Freight
{
public:
    virtual ~OceanFreight() = default;

    virtual double volumetric_weight(Centimeter l, Centimeter w, Centimeter h, unsigned int packages) const override
    { return l * w * h / 1000.0 * packages; }
} const ocean_freight;
class RailFreight
    : public Freight
{
public:
    virtual ~RailFreight() = default;

    virtual double volumetric_weight(Centimeter l, Centimeter w, Centimeter h, unsigned int packages) const override
    { return l * w * h / 3000.0 * packages; }
} const rail_freight;

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

    void setEmail(const std::string& email) {
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

        cost = 0.0;
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

int main()
{
    return 0;
}
