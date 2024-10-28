#include <iostream>
#include <format>

#define fn auto

#define setter(_type, _name) \
    fn set_##_name(_type _##_name) noexcept -> void { this->m_##_name = std::move(_##_name); }
#define getter(_type, _name) \
    fn get_##_name() const noexcept -> _type { return this->m_##_name; }

class RoomDimension {
private:
    double m_length, m_width;

public:
    RoomDimension(double _length, double _width)
        : m_length(_length), m_width(_width)
    {}
    setter(double, length)
    setter(double, width)
    getter(double, length)
    getter(double, width)
    constexpr fn area() const noexcept -> double { return m_length * m_width; }
};

class RoomCarpet {
private:
    RoomDimension m_dimensions;
    double m_cost_per_sqft;

public:
    RoomCarpet(RoomDimension _dimensions, double _cost_per_sqft)
        : m_dimensions(_dimensions), m_cost_per_sqft(_cost_per_sqft)
    {}
    setter(RoomDimension, dimensions)
    setter(double, cost_per_sqft)
    getter(RoomDimension, dimensions)
    getter(double, cost_per_sqft)
    constexpr fn total_cost() const noexcept -> double { return m_dimensions.area() * m_cost_per_sqft; }
};

fn main() -> int
{
    double length, width, cost_per_sqft;
    std::cout << "Enter the length of the room: ";
    std::cin >> length;
    std::cout << "Enter the width of the room: ";
    std::cin >> width;
    std::cout << "Enter the cost per square foot of the carpet: ";
    std::cin >> cost_per_sqft;

    const RoomCarpet room{{length, width}, cost_per_sqft};
    std::cout << std::format("The total cost of the carpet is: {}\n", room.total_cost());
}
