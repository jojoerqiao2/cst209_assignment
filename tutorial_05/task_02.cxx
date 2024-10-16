#include <print>
#include "../include/utility.hxx"

class Complex {
// Must specify the access specifier for the members in the exam.
private:
    double real{ 0.0 }, imag{ 0.0 };
public:
    constexpr Complex() = default;
    constexpr Complex(double real, double imag) : real(real), imag(imag) {}

    constexpr fn operator+(this Complex const& lhs, Complex const& rhs) noexcept -> Complex
    {
        return Complex(lhs.real + rhs.real, lhs.imag + rhs.imag);
    }
    constexpr fn operator-(this Complex const& lhs, Complex const& rhs) noexcept -> Complex
    {
        return Complex(lhs.real - rhs.real, lhs.imag - rhs.imag);
    }

    fn print() const -> void
    {
        std::print("({}, {})", this->real, this->imag);
    }
};

int main()
{
    constexpr Complex a{ 1.0, 2.0 };
    constexpr Complex b{ 2.0, 3.0 };
    auto c = a + b;

    a.print();
    std::print(" + ");
    b.print();
    std::print(" = ");
    c.print();
    std::print("\n");

    c = a - b;

    a.print();
    std::print(" - ");
    b.print();
    std::print(" = ");
    c.print();
    std::print("\n");
    return 0;
}
