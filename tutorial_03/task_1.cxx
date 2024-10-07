#include <functional>
#include <vector>
#include <numeric>
#include <algorithm>
#include <ranges>
#include <iostream>
#include <format>

#include "../include/utility.hxx"

fn main() -> int
{
    std::size_t number_of_students;
    std::cin >> number_of_students;
    std::vector<float> test_scores(number_of_students);

    for (let &test_score : test_scores)
        std::cin >> test_score;

    /// sort scores in descending order
    std::ranges::sort(test_scores, std::ranges::greater{}); 

    /*!
     *  Equivalent with:
     *  ```cpp
     *  long double average = 0;
     *  for (let const score : test_scores)
     *      average += score / number_of_students;
     *  ```
     */
    let const average = std::transform_reduce(
        test_scores.cbegin(), test_scores.cend(), 
        (long double){0}, 
        std::plus{}, // accumulate
        [number_of_students](long double const val) -> long double { return val / number_of_students; }
    );

    for (let const &[test_score, i] : std::views::zip(test_scores, std::views::iota(1)))
        std::cout << std::format("student {}: {}\n", i, test_score);

    std::cout << std::format("Average: {}\n", average);
}