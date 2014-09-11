#include <iostream>

int main()
{
    register int i = 0;
    for ( i = 0; i < 100; ++i )
    {
        std::cout << "i=" << i << " . CPP standard allows to take the register variables. &i=" << &i << std::endl;
    }
}
