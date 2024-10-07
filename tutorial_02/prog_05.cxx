#include <print>
#define fn  auto
#define let auto

fn main() -> int 
{
    {
        int i = 30;
        while (i >= -30) {
            static bool first_run = true;
            std::print(
                "{}{}", 
                (first_run ? ((first_run = false), "") : ", "),
                i
            );
            i -= 10;
        }        
        std::println();
    }

    {
        int i = 15;
        while (i <= 55) {
            static bool first_run = true;
            std::print(
                "{}{}", 
                (first_run ? ((first_run = false), "") : ", "),
                i
            );
            i += 8;
        }        
        std::println();
    }
}