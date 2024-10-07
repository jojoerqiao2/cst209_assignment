#include <cstdint>
#include <iostream>
#include <format>
#define fn  auto
#define let auto

fn main() -> int 
{
    using uint128_t = __uint128_t;
    let const Get_Assessment_Value = 
        [](std::uint64_t actual_value) -> std::uint64_t
        {
            let const assessed_value = (uint128_t)actual_value * 3 / 5;
            return (std::uint64_t)assessed_value;
        };
    let const Get_Property_Tax = 
        [](std::uint64_t assessment_value) -> std::uint64_t
        {
            let const property_tax = (uint128_t)assessment_value * 3 / 4;
            return (std::uint64_t)property_tax;
        };
    let actual_value = std::uint64_t{};

    std::cout << "What's the value of your property? ";
    std::cin >> actual_value;
    
    let const assessment_value = Get_Assessment_Value(actual_value);
    let const property_tax = Get_Property_Tax(assessment_value);
    std::cout << std::format(
        "The assessment value of your property: {}\n"
        "The property tax for that: {}\n",
        assessment_value, property_tax
    );
}