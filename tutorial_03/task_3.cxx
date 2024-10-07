#include <vector>
#include <algorithm>
#include <iostream>
#include <format>

#include "../include/utility.hxx"

fn main(void) -> int
{
    std::vector<int> numbers;
    for (int i = 0; i < 20; ++i) {
        int number;
        std::cin >> number;
        /*!
         *  WARNING: As each number is read, validate it and store it in the vector ONLY IF it isn’t a duplicate of a number already read.
         */
        if (!std::ranges::binary_search(numbers, number)) {
            numbers.push_back(number);
            /*!
             *  Use partial_sort instead of sort since only the last number is unordered.
             */
            std::ranges::partial_sort(numbers, std::ranges::prev(numbers.end()));
        }
    }
    for (let const &number : numbers)
        std::cout << std::format("{} ", number);
    std::cout << '\n';
}