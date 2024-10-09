#include <string>
#include <chrono>
#include <iostream>
#include <print>
#include "../include/utility.hxx"

class HeartRates {
    std::string m_first_name, m_last_name;
    std::chrono::year m_birth_year;

public:
    struct MinMaxHeartRate {
        int min, max;
    };

    constexpr HeartRates() = default;
    constexpr HeartRates(std::string first_name, std::string last_name, std::chrono::year birth_year)
        : m_first_name(first_name), m_last_name(last_name), m_birth_year(birth_year)
    {}
    
    fn set_first_name(std::string first_name) -> void
    { this->m_first_name = std::move(first_name); }
    fn set_last_name(std::string last_name) -> void
    { this->m_last_name = std::move(last_name); }
    fn set_birth_year(std::chrono::year birth_year) -> void
    { this->m_birth_year = birth_year; }

    fn get_first_name() const -> std::string
    { return this->m_first_name; }
    fn get_last_name() const -> std::string
    { return this->m_last_name; }
    fn get_birth_year() const -> std::chrono::year
    { return this->m_birth_year; }

    /// Too crazy for getting the current year in C++ with only STL.
    static fn get_current_year() -> std::chrono::year
    { return std::chrono::year_month_day{floor<std::chrono::days>(std::chrono::system_clock::now())}.year(); }

    fn getAge(std::chrono::year current_year = get_current_year()) const -> int
    { return (int)current_year - (int)this->m_birth_year; }
    
    fn getMaximumHeartRate() -> int
    { return 220 - (int)this->getAge(); }

    fn getTargetHeartRate() -> MinMaxHeartRate
    {
        let const max_heart_rate = this->getMaximumHeartRate();
        return { (int)(max_heart_rate * 0.5), (int)(max_heart_rate * 0.85) };
    }
};

fn main() -> int
{
    std::string first_name, last_name;
    int birth_year;
    std::cin >> first_name >> last_name >> birth_year;
    
    HeartRates person{ first_name, last_name, std::chrono::year(birth_year) };
    let const heart_rate_range = person.getTargetHeartRate();
    std::println(
        "Name: {} {}\n"
        "Birth Year: {}\n"
        "Age: {}\n"
        "Maximum Heart Rate: {}\n"
        "Target Heart Rate Range: {} - {}",
        person.get_first_name(), person.get_last_name(),
        person.get_birth_year(),
        person.getAge(),
        person.getMaximumHeartRate(),
        heart_rate_range.min, heart_rate_range.max
    );
}
