#include <print>
#include "../include/utility.hxx"
class Account {
    int balance;

public:
    constexpr Account() = default;
    constexpr Account(int arg_balance) 
        : balance(arg_balance > 0 ? arg_balance : 0) 
    {
        if (arg_balance < 0)
            std::println(stderr, "Account balance cannot be negative. Setting balance to 0.");
    }

    constexpr fn credit(int amount) -> void
    {
        if (amount < 0)
            std::println(stderr, "Credit amount cannot be negative.");
        this->balance += amount;
    }

    constexpr fn debit(int amount) -> void
    {
        if (amount > balance)
            std::println(stderr, "Debit amount exceeded account balance.");
        this->balance -= amount;
    }
    constexpr fn getBalance() const -> int
    { return this->balance; }
};
