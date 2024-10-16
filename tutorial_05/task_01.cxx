#include <vector>
#include <iostream>
#include <format>
#include <cstdint>

#include "../include/utility.hxx"

class NumDays {
private:
    std::uint32_t hours;

    static fn hours_to_days(std::uint32_t hours) -> double
    {
        return hours / 8.0;
    }
public:
    constexpr NumDays(std::uint32_t hours) : hours(hours) {}
    constexpr fn get_hours() const noexcept -> std::uint32_t { return hours; }
    constexpr fn get_days() const noexcept -> double { return hours_to_days(hours); }
};

int main()
{
    std::uint32_t hours = 0;
    std::vector<NumDays> nds;
    for (int i = 0; i < 3; ++i) {
        std::cout << "Working hours: ";
        std::cin >> hours;
        nds.push_back(NumDays(hours));
        const auto& nd = nds.back();
        std::cout << std::format("Working days: {}\n\n", nd.get_days());
    }
}
