#include <cstdint>
#include <format>
#include <iostream>
#include "../include/utility.hxx"

class Time {
private:
    std::uint32_t m_hours{}, m_minutes{}, m_seconds{};
    static constexpr std::uint32_t seconds_in_minute = 60;
    static constexpr std::uint32_t seconds_in_hour = 3600;
    static constexpr std::uint32_t minutes_in_hour = 60;
public:
    constexpr Time() noexcept = default;
    constexpr Time(std::uint32_t seconds) noexcept
        : m_hours(seconds / seconds_in_hour),
          m_minutes((seconds % seconds_in_hour) / seconds_in_minute),
          m_seconds(seconds % seconds_in_minute)
    {}
    constexpr fn operator+(this Time const& lhs, Time const& rhs) noexcept -> Time
    {
        return Time(
            (lhs.m_hours + rhs.m_hours) * seconds_in_hour +
            (lhs.m_minutes + rhs.m_minutes) * seconds_in_minute +
            lhs.m_seconds + rhs.m_seconds
        );
    }
    constexpr fn operator-(this Time const& lhs, Time const& rhs) noexcept -> Time
    {
        return Time(
            (lhs.m_hours - rhs.m_hours) * seconds_in_hour +
            (lhs.m_minutes - rhs.m_minutes) * seconds_in_minute +
            lhs.m_seconds - rhs.m_seconds
        );
    }
    fn print() const -> void
    {
        std::cout << std::format(
            "{:02}:{:02}:{:02}",
            this->m_hours, this->m_minutes, this->m_seconds
        );
    }
};
