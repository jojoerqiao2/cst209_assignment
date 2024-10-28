#include <iostream>
#include <utility>
#include <format>
#include <string>
#include <string_view>

#define fn auto

#define setter(_type, _name) \
    fn set_##_name(_type _##_name) noexcept -> void { this->_name = std::move(_##_name); }
#define getter(_type, _name) \
    fn get_##_name() const noexcept -> _type { return this->_name; }

class RetailItem {
protected:
    std::string description;
    int unitsOnHand;
    double price;
public:
    RetailItem(std::string_view _description, int _unitsOnHand, double _price)
        : description(_description), unitsOnHand(_unitsOnHand >= 0 ? _unitsOnHand : 0), price(_price)
    {}
    setter(std::string, description)
    setter(int, unitsOnHand)
    setter(double, price)
    getter(std::string, description)
    getter(int, unitsOnHand)
    getter(double, price)
};

class CashRegister {
private:
    static constexpr double tax_rate = 6e-2;
    class RetailItem item;
    int quantity;
public:
    CashRegister(class RetailItem _item, int _quantity)
        : item(std::move(_item)), quantity(_quantity < _item.get_unitsOnHand() ? _quantity : 0)
    {}
    fn getSubtotal() const noexcept -> double { return item.get_price() * quantity; }
    fn getTax() const noexcept -> double { return getSubtotal() * tax_rate; }
    fn getTotal() const noexcept -> double { return getSubtotal() + getTax(); }
};

fn main() -> int
{
    static const RetailItem item{"Jacket", 114514, 15.5};
    int quantity;
    std::cout << "Enter purchase quantity: ";
    std::cin >> quantity;
    CashRegister r{item, quantity};
    std::cout << std::format(
        "The subtotal: ${:.1f}\n"
        "The tax: ${:.2f}\n"
        "The total: ${:.2f}\n",
        r.getSubtotal(), r.getTax(), r.getTotal()
    );
}
