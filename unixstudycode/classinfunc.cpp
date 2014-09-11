#include <iostream>

void func()
{
    class SS
    {
    public:
        SS()
        {
            std::cout << "a class in function" << std::endl;
        }
    };


    SS s;

    std::cout << "aaaaaaaaaaaaaaaaaaa" << std::endl;

}

int main( int argc, char* argv[] )
{
    func();
    return 0;
}
