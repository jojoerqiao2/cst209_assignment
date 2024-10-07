#include <iostream>
#include <format>
#define fn  auto
#define let auto

fn main() -> int 
{
    let const suger_per_cookie = double{ 1.5 / 48 };
    let const butter_per_cookie = double{ 1.0 / 48 };
    let const flour_per_cookie = double{ 2.75 / 48 };
    let number_of_cookies = std::size_t{};

    std::cout << "How many cookies do you want to make? ";
    std::cin >> number_of_cookies;
    std::cout << std::format(
        "You need these for your requirement:\n"
        "    {} cups of sugar\n"
        "    {} cups of butter\n"
        "    {} cups of flour\n",
        number_of_cookies * suger_per_cookie, 
        number_of_cookies * butter_per_cookie, 
        number_of_cookies * flour_per_cookie
    );
}