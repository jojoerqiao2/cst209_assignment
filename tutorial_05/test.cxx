#include <cstdio>
using namespace std; 
 
class Sample { 
    static inline int count = 0; 

public: 
    Sample() noexcept
    { count++; }
    void printTotalObj_nonstatic() const noexcept
    { 
        printf("%d\n", count); 
    }
    static void printTotalObj_static() noexcept
    {
        printf("%d\n", count); 
    }
};
 
int main() 
{ 
    const Sample S1; 
    const Sample S2; 
 
    // Call to non-static member function without an object argument
    Sample::printTotalObj_static();
    S1.printTotalObj_nonstatic();
    return 0; 
} 
