#include <array>
#include <stack>
#include <string>

#define fn  auto
#define let auto

fn main() -> int
{
    using namespace std::string_literals;
    let constexpr arr = std::array {
        "C++"s,
        "Java"s,
        "Python"s,
    };
    std::stack<std::string> st { arr.cbegin(), arr.cend() };  
}