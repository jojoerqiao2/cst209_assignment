#include <print>
#define fn  auto
#define let auto

fn main() -> int
{
    let sum = int{};
    for (int i = 7; i <= 100; i+=7)
        sum += i;
    std::println("{}", sum);
}