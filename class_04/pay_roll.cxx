#include <cstdint>
#include <print>
#include <string>

#define fn  auto
#define let auto

struct PayRoll {
    std::uint64_t ID;
    std::string name, gender;
    double hours, pay_rate;
};

fn main() -> int 
{
    PayRoll employee{
        41123,
        "George",
        "Male",
        45.5,
        165.50,
    };
    std::print(
        "\"ID\": {},\n"
        "\"name\": {},\n"
        "\"gender\": {},\n"
        "\"hour\": {},\n"
        "\"pay_rate\": {}\n",
        employee.ID, employee.name, employee.gender, employee.hours, employee.pay_rate
    );
}