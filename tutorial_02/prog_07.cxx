#include <print>
#include <iostream>
#include <string>
#include <cctype>
#define fn  auto
#define let auto

fn Remove_Nonprintable(std::string str) -> std::string
{
    std::string striped_str;
    for (let const ch : str)
        if (std::isprint(ch))
            striped_str.push_back(ch);
    return striped_str;
}

fn Advance() -> void
{
    while (!std::isprint(std::cin.peek()))
        std::cin.get();
}

fn main() -> int 
{
    double cost, total [[maybe_unused]];
    std::print("Cost of items: ");
    std::cin >> cost;
    Advance();

    std::string voucher_code;
    std::print("Voucher code: ");
    std::cin >> voucher_code;
    Advance();

    std::string shipping_opt;
    std::print("Shipping option: ");
    std::cin >> shipping_opt;
    Advance();

    bool damage_protection_opt;
    {
        char ch;
        std::cin >> ch;
        switch (ch) {
        case 'N':
        case 'n':
            damage_protection_opt = false;
            break;
        default:
            damage_protection_opt = true;
            break;
        }
    }
}