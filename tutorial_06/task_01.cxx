#include <cstdint>
#include <format>
#include <iostream>
#include <string_view>
#include <random>
#define fn auto

class Coin {
public:
    static constexpr char const * const heads = "heads";
    static constexpr char const * const tails = "tails";

private:
    std::string_view sideUp;

    static constexpr fn toss_impl() noexcept -> std::string_view
    {
        static thread_local std::random_device rd;
        static thread_local std::ranlux24_base gen(rd());
        static thread_local std::uniform_int_distribution<std::uint8_t> dist{0, 1};
        return dist(gen) ? heads : tails;
    }

public:
    Coin() noexcept;
    fn toss() noexcept -> void;
    fn getSideUp() const noexcept -> std::string_view;
};

Coin::Coin() noexcept : sideUp(toss_impl()) {}
fn Coin::toss() noexcept -> void { sideUp = toss_impl(); }
fn Coin::getSideUp() const noexcept -> std::string_view { return sideUp; }

fn main() -> int
{
    Coin coin{};
    std::cout << std::format("The side up is: {}\n", coin.getSideUp());

    int heads_count = 0, tails_count = 0;
    for (int i = 0; i < 20; ++i) {
        coin.toss();
        heads_count += coin.getSideUp() == Coin::heads;
        tails_count += coin.getSideUp() == Coin::tails;
        std::cout << std::format("The side up is: {}\n", coin.getSideUp());
    }
    std::cout << std::format("Heads count: {}\n", heads_count);
    std::cout << std::format("Tails count: {}\n", tails_count);
}
