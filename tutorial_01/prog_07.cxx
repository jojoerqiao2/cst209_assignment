#include <utility>
#include <iostream>
#include <format>
#include <array>
#define fn  auto
#define let auto

namespace cst209
{
    template <typename T>
    fn swap(T& first, T& second) -> void
    {
        T temp = std::move(first);
        first = std::move(second);
        second = std::move(temp);
    }
}

fn main() -> int 
{
    let static constexpr banner = 
        "Swap two numbers:\n"
        "-----------------------\n";
    let static constexpr output_fmt_str =
        "After swapping, the 1st number is: {}\n"
        "After swapping, the 2nd number is: {}\n";

    std::array<long double, 2> num = {};
    std::cout << banner;
    std::cout << "Input 1st number: ";
    std::cin >> num[0];
    std::cout << "Input 2nd number: ";
    std::cin >> num[1];
    cst209::swap(num[0], num[1]);
    std::cout << std::format(
        output_fmt_str,
        num[0], num[1]
    );
}