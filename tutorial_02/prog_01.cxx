#include <compare>
#include <exception>
#include <iostream>
#define fn  auto
#define let auto

fn main() -> int 
{
    double a{}, b{}, c{};
    std::cin >> a >> b >> c;
    let const D = b * b - 4 * a * c;
    let const cmp_result = D <=> 0.0;
    if (cmp_result == std::partial_ordering::greater) {
        std::cout << "The equation has two roots.\n";
    } else if (cmp_result == std::partial_ordering::equivalent) {
        std::cout << "The equation has one root.\n";
    } else if (cmp_result == std::partial_ordering::less) {
        std::cout << "The equation has no real roots.\n";
    } else {
        std::clog << "Numeric error!\n" << "Abort...\n";
        std::terminate();
    }
}