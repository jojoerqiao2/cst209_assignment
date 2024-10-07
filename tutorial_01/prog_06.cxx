#include <iostream>
#include <format>
#include <cstdint>
#define fn  auto
#define let auto

fn main() -> int
{
    let const payAmount = double{ 2200.0 };
    let const payPeriods = std::uint32_t{ 26 };
    let const annualPay = payAmount * payPeriods;
    std::cout << std::format(
        "Annual pay: RM{:.2f}\n",
        annualPay
    );
}