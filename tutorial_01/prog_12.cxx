#include <iostream>
#include <format>
#include <cmath>
#define fn  auto
#define let auto

fn main() -> int
{
    let annual_interest_rate = double{};
    let number_of_payments = std::size_t{};
    let loan_amount = double{};
    let static constexpr report_fmt_str = 
        "Loan Amount: RM{:.2f}\n"
        "Monthly Interest Rate: {}%\n"
        "Number of Payments: {}\n"
        "Monthly Payment: RM{:.2f}\n"
        "Amount Paid Back: RM{:.2f}\n"
        "Interest Paid: RM{:.2f}\n";
    
    std::cout << "Please enter these data for loan calculation:\n";
    std::cout << "    Annual interest rate: ";
    std::cin >> annual_interest_rate;
    std::cout << "    Number of payments: ";
    std::cin >> number_of_payments;
    std::cout << "    Loan amount: ";
    std::cin >> loan_amount;

    let const monthly_interest_rate = annual_interest_rate / 12;
    let const monthly_payment = 
        (monthly_interest_rate * std::pow(1.0 + monthly_interest_rate, number_of_payments)) /
        (std::pow(1.0 + monthly_interest_rate, number_of_payments) - 1) *
        loan_amount;
    let const amount_paid_back = monthly_payment * number_of_payments;
    let const interest_paid = amount_paid_back - loan_amount;

    std::cout << std::format(
        report_fmt_str,
        loan_amount, 
        monthly_interest_rate, 
        number_of_payments,
        monthly_payment,
        amount_paid_back,
        interest_paid
    );
}