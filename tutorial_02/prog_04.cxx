#include <print>
#define fn  auto
#define let auto

fn main() -> int 
{
    for (int i = 1; i <= 13; i+=2) {
        static bool first_run = true;
        std::print(
            "{}{}", 
            (first_run ? ((first_run = false), "") : ", "),
            i
        );
    }
    std::println();
    
    for (int i = 2; i <= 17; i+=3) {
        static bool first_run = true;
        std::print(
            "{}{}", 
            (first_run ? ((first_run = false), "") : ", "),
            i
        );
    }
    std::println();
}