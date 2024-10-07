#include <iostream>
#include <format>
#define fn  auto
#define let auto

fn main() -> int
{
    let const sales_tax_rate = double{ 4E-2 };
    let const service_tax_rate = double{ 2E-2 };
    // plus or multiple?
    let const total_tax_rate = sales_tax_rate + service_tax_rate;
    let const sales = double{ 95.00 };
    std::cout << std::format(
        "Total sales tax: {:.2f}\n", 
        sales * total_tax_rate
    );
}