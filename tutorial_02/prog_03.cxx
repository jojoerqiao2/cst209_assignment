#include <iostream>
#include <algorithm>
#include <limits>
#include <array>
#define fn  auto
#define let auto

struct Tax {
    double min_income, max_income, rate;
};
let constexpr tax_category = std::array{
    Tax{ 0, 5'000, 0 },
    Tax{ 5'001, 20'000, 1E-2 },
    Tax{ 20'001, 35'000, 3E-2 },
    Tax{ 35'001, 50'000, 6E-2 },
    Tax{ 50'001, 70'000, 11E-2 },
    Tax{ 70'001, 100'000, 19E-2 },
    Tax{ 100'001, 400'000, 25E-2 },
    Tax{ 400'001, 600'000, 26E-2 },
    Tax{ 600'001, 2'000'000, 28E-2 },
    Tax{ 2'000'001, std::numeric_limits<double>::max(), 30E-2 },
};

fn main() -> int 
{
    double income{}, tax{ 0.0 };
    std::cin >> income;
    for (let const &tax_level : tax_category) {
        tax += (income >= tax_level.min_income) * (std::min(income, tax_level.max_income) - tax_level.min_income) * tax_level.rate;
    }
    std::cout << tax << std::endl;
}