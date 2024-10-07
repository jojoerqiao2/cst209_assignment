#include <numbers>

#define fn  auto
#define let auto

class Circle {
private:
    double radius;
public:
    constexpr Circle() = default;
    constexpr Circle(Circle const&) noexcept = default;
    constexpr Circle(Circle &&) noexcept = default;
    constexpr Circle(double r) : radius{r} {}
    
    constexpr fn set_radius(double r) noexcept -> void
    { this->radius = r; }
    constexpr fn get_radius() const noexcept -> double
    { return this->radius; }
    constexpr fn calc_area() const noexcept -> double
    { return std::numbers::pi * this->radius * this->radius; }
};

fn main() -> int {}