#include <vector>
#include <algorithm>
#include <iostream>
#include <format>

#include "../include/utility.hxx"

fn main() -> int
{
    std::vector account_list {
        5658845, 4520125, 7895122, 8777541, 8451277, 1302850,
        8080152, 4562555, 5552012, 5050552, 7825877, 1250255,
        1005231, 6545231, 3852085, 7576651, 7881200, 4581002,
    };
    std::ranges::sort(account_list);

    int target_account;
    std::cin >> target_account;

    let const is_valid = std::ranges::binary_search(account_list, target_account);

    std::cout << std::format("{} is {} account.\n", target_account, is_valid ? "a valid" : "an invalid");
}