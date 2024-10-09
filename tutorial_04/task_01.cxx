#include <utility>
#include <string>
#include <string_view>
#include <cstdint>
#include <vector>
#include <print>
#include "../include/utility.hxx"

class Employee {
    std::string name, department, position;
    std::uint32_t idNumber;
public:
    std::string_view static constexpr fmt_str = 
        "{{\n"
        "    \"name\": \"{}\",\n"
        "    \"department\": \"{}\",\n"
        "    \"position\": \"{}\",\n"
        "    \"idNumber\": {}\n"
        "}}";

    Employee() = default;
    Employee(std::string name, std::uint32_t idNumber, std::string department = {}, std::string position = {})
        : name(name), department(department), position(position), idNumber(idNumber)
    {}

    fn set_name(std::string name) -> void
    { this->name = std::move(name); }
    fn set_department(std::string department) -> void
    { this->department = std::move(department); }
    fn set_position(std::string position) -> void
    { this->position = std::move(position); }
    fn set_idNumber(std::uint32_t idNumber) -> void
    { this->idNumber = idNumber; }

    fn get_name() const -> std::string
    { return this->name; }
    fn get_department() const -> std::string
    { return this->department; }
    fn get_position() const -> std::string
    { return this->position; }
    fn get_idNumber() const -> std::uint32_t
    { return this->idNumber; }
};

fn main() -> int
{
    std::vector<Employee> const employees = {
        Employee{"Susan Meyers", 47899, "Accounting", "Vice President"},
        Employee{"Mark Jones", 39119, "IT", "Programmer"},
        Employee{"Joy Rogers", 81774, "Manufacturing", "Engineer"},
    };
    
    for (auto const& employee : employees) {
        std::println(
            Employee::fmt_str, 
            employee.get_name(), employee.get_department(), employee.get_position(), employee.get_idNumber()
        );
    }
}
