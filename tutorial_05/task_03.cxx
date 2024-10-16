#include <print>

#include "../include/utility.hxx"

class Circle {
private:
    double radius{ 0 }, area{ 0 }, diameter{ 0 }, circumference{ 0 };
    static constexpr double pi = 3.14159;

    constexpr static fn calcArea(double r) noexcept -> double
    { return pi * r * r; }
    constexpr static fn calcDiameter(double r) noexcept -> double
    { return 2 * r; }
    constexpr static fn calcCircumference(double r) noexcept -> double
    { return 2 * pi * r; }
public:
    constexpr Circle() = default;
    constexpr Circle(double r)
        : radius(r), area(calcArea(r)), diameter(calcDiameter(r)), circumference(calcCircumference(r))
    {}

    constexpr fn setRadius(double r) noexcept -> void
    {
        radius = r;
        area = calcArea(r);
        diameter = calcDiameter(r);
        circumference = calcCircumference(r);
    }

    constexpr fn getRadius() const noexcept -> double
    { return radius; }
    constexpr fn getArea() const noexcept -> double
    { return area; }
    constexpr fn getDiameter() const noexcept -> double
    { return diameter; }
    constexpr fn getCircumference() const noexcept -> double
    { return circumference; }

};

int main()
{
    constexpr Circle c1{ 3.5 };
    Circle c2{};
    c2.setRadius(5.2);

    std::print(
        "Circle 1\n"
        "Area: {:.4f}\n"
        "Diameter: {}\n"
        "Circumference: {:.4f}\n\n",
        c1.getArea(), c1.getDiameter(), c1.getCircumference()
    );
    std::print(
        "Circle 2\n"
        "Area: {:.4f}\n"
        "Diameter: {}\n"
        "Circumference: {:.4f}\n\n",
        c2.getArea(), c2.getDiameter(), c2.getCircumference()
    );
}
