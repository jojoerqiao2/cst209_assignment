#include <format>
#include <iostream>
#include <vector>
#include <algorithm>

#define fn  auto
#define let auto

fn main() -> int
{
    std::vector rainfalls(12, 0);
    std::cout << "Enter the rainfall of each month:\n";
    for (let &rainfall : rainfalls) {
        std::cin >> rainfall;
    }
    let const [lowest, highest] = std::ranges::minmax(rainfalls);
    std::cout << std::format("Minimum: {}\nMaximum: {}\n", lowest, highest);
}

// Input: 25 23 55 92 52 103 208 499 380 148 88 62