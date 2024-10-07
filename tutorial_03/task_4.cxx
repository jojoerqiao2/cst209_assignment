#include <vector>
#include <random>
#include <cstdint>
#include <print>

#include "../include/utility.hxx"

fn dice() -> std::uint8_t
{
    static std::random_device rd{};
    static thread_local std::ranlux24_base gen{rd()};
    static thread_local std::uniform_int_distribution<std::uint8_t> dist{1, 6};
    return dist(gen);
}

fn main() -> int
{
    let constexpr dice_times = 36000;
    std::vector dice_result_occurrence(12, 0zu);
    for (std::size_t time = 0; time < dice_times; ++time) {
        /*!
         *  The result of each dice can only of range [1, 6].
         *  Therefore the ${dice_result} can only of range [2, 12].
         */
        let const dice_result = dice() + dice();
        [[assume(2 <= dice_result && dice_result <= 12)]];
        ++dice_result_occurrence[dice_result];
    }
    for (std::uint8_t result = 2; result <= 12; ++result) {
        std::println("Frequency of {}: {}", result, dice_result_occurrence[result]);
    }
}