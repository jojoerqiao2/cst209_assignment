#include <iostream>
#include <format>
#include <array>
#define fn  auto
#define let auto

fn main() -> int 
{
    let const price_list = std::array {
        15.95,
        24.95,
        6.95,
        12.95,
        3.95,
    };
    let const sales_tax_rate = double{ 7.5E-2 };
    let sales_subtotal = double{ 0.0 };
    let sales_tax_amount = double{ 0.0 };

    for (std::size_t i = 0; i < price_list.size(); ++i) {
        let const &item_price = price_list[i];
        sales_subtotal += item_price;
        sales_tax_amount += item_price * sales_tax_rate;
        std::cout << std::format(
            "Price of item {} = RM{:.2f}\n",
            i, item_price
        );
    }
    let const total = sales_subtotal + sales_tax_amount;

    std::cout << std::format(
        "The sale subtotal = RM{:.2f}\n"
        "The sales tax amount = RM{:.2f}\n"
        "The total = RM{:.2f}\n",
        sales_subtotal, sales_tax_amount, total
    );
}