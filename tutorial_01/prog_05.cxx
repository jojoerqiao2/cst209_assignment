#include <iostream>
#include <format>
#define fn  auto
#define let auto

fn main() -> int
{
    let const meal_cost = double{ 88.67 };
    let const tax_rate = double{ 6.75E-2 };
    let const tip_rate = double{ 20E-2 };
    let const tax_amount = meal_cost * tax_rate;
    let const tip_amount = (meal_cost + tax_amount) * tip_rate;
    let const total_bill = meal_cost + tax_amount + tip_amount;
    std::cout << std::format(
        "meal cost: RM{:.2f},\n"
        "tax amount: RM{:.2f},\n"
        "tip amount: RM{:.2f},\n"
        "total bill: RM{:.2f}\n",
        meal_cost, tax_amount, tip_amount, total_bill
    );
}