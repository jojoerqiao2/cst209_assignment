#include <string>
#include <string_view>

#include <iostream>
#include <format>

class Shape
{
private:
    std::string m_name{};
public:
    Shape() = default;
    Shape(std::string_view name) : m_name(name) {}
    virtual ~Shape() = default;

    auto get_name() const -> std::string
    { return m_name; }
    void set_name(std::string_view name)
    { m_name = name; }
};

class Rectangle 
    : public Shape
{
private:
    double m_width{};
    double m_height{};
public:
    Rectangle() = default;
    Rectangle(std::string_view name, double width, double height)
        : Shape(name), m_width(width), m_height(height) 
    {}
    virtual ~Rectangle() = default;

    auto get_width() const -> double
    { return m_width; }
    auto get_height() const -> double
    { return m_height; }
    auto get_area() const -> double
    { return m_width * m_height; }
    void set_width(double width)
    { m_width = width; }
    void set_height(double height)
    { m_height = height; }
};

class Square
    : public Rectangle
{
public:
    Square() = default;
    Square(std::string_view name, double side)
        : Rectangle(name, side, side)
    {}
    virtual ~Square() = default;

    void set_side(double side)
    {
        Rectangle::set_width(side);
        Rectangle::set_height(side);
    }
};

int main()
{
    Rectangle rectangle{"Rectangle", 2.0, 3.0};
    std::cout << std::format(
        "{}: width = {}, height = {}, area = {}\n",
        rectangle.get_name(), rectangle.get_width(), rectangle.get_height(), rectangle.get_area()
    );
    Square square{"Square", 2.0};
    square.
    return 0;
}
