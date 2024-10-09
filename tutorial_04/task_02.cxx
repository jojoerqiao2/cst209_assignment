#include <string_view>
#include <print>
#include "../include/utility.hxx"


class Inventory {
    int itemNumber;
    int quantity;
    double cost;
    double totalCost;

public:
    std::string_view static constexpr fmt_str = 
        "{{\n"
        "    \"itemNumber\": {},\n"
        "    \"quantity\": {},\n"
        "    \"cost\": {},\n"
        "    \"totalCost\": {}\n"
        "}}";

    constexpr Inventory() = default;
    constexpr Inventory(int itemNumber, int quantity, double cost)
        : itemNumber(itemNumber), quantity(quantity), cost(cost), totalCost(quantity * cost)
    {}

    constexpr fn setItemNumber(int itemNumber) -> void
    { this->itemNumber = itemNumber; }
    constexpr fn setQuantity(int quantity) -> void
    { 
        this->quantity = quantity;
        this->setTotalCost();
    }
    constexpr fn setCost(double cost) -> void
    { 
        this->cost = cost;
        this->setTotalCost();
    }
    constexpr fn setTotalCost() -> void
    { this->totalCost = this->quantity * this->cost; }
    
    constexpr fn getItemNumber() const -> int
    { return this->itemNumber; }
    constexpr fn getQuantity() const -> int
    { return this->quantity; }
    constexpr fn getCost() const -> double
    { return this->cost; }
    constexpr fn getTotalCost() const -> double
    { return this->totalCost; }
};

fn main() -> int
{
    Inventory item{ 2983, 4, 217.5 };
    std::println(
        Inventory::fmt_str, 
        item.getItemNumber(), item.getQuantity(), item.getCost(), item.getTotalCost()
    );
    
    std::println("Setting quantity to 10");
    item.setQuantity(10);
    std::println(
        Inventory::fmt_str, 
        item.getItemNumber(), item.getQuantity(), item.getCost(), item.getTotalCost()
    );

    std::println("Setting cost to 300.0");
    item.setCost(300.0);
    std::println(
        Inventory::fmt_str, 
        item.getItemNumber(), item.getQuantity(), item.getCost(), item.getTotalCost()
    );

    std::println("Setting item number to 1234");
    item.setItemNumber(1234);
    std::println(
        Inventory::fmt_str, 
        item.getItemNumber(), item.getQuantity(), item.getCost(), item.getTotalCost()
    );
}
