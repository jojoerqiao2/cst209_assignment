#include <iostream>
#include <format>
#include <array>
#define fn  auto
#define let auto

fn main() -> int 
{
    let constexpr months = std::size_t{ 12 };
    let rainfall_per_month = std::array<double, months>{};
    let constexpr names_of_months = std::array<const char*, months> {
        "January",
        "February",
        "March",
        "April",
        "May",
        "June",
        "July",
        "August",
        "September",
        "October",
        "November",
        "December",
    };

    for (std::size_t i = 0; i < months; ++i) {
        let const &name_of_month = names_of_months[i];
        let &rainfall_this_month = rainfall_per_month[i];
        let rainfall = double{};
        std::cout << std::format("Please key in the rainfall of {}: ", name_of_month);
        std::cin >> rainfall;
        if (rainfall <= 0.0) {
            std::clog << "Rainfall must be positive!\n";
            std::clog << "Abort...\n";
            std::terminate();
        }
        rainfall_this_month = rainfall;
    }

    let total_rainfall = double{ 0.0 };
    for (let const &rainfall : rainfall_per_month) {
        total_rainfall += rainfall;
    }
    let const average_rainfall = total_rainfall / months;

    std::cout << std::format(
        "Total rainfall: {}\n"
        "Average monthly rainfall: {}\n",
        total_rainfall, average_rainfall
    );
}