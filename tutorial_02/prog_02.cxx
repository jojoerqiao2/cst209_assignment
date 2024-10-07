#include <iostream>
#define fn  auto
#define let auto

fn main() -> int 
{
    let const normal_hour_salary = double{ 120.0 };
    let const max_normal_hour = std::size_t{ 40 };
    let const extra_hour_salary = double{ 1.5 * normal_hour_salary };
    let working_hour = std::size_t{};
    std::cin >> working_hour;
    let const salary = 
        (working_hour <= max_normal_hour) ? 
        (working_hour * normal_hour_salary) : 
        (max_normal_hour * normal_hour_salary + (working_hour - max_normal_hour) * extra_hour_salary);
    std::cout << salary << std::endl;
}