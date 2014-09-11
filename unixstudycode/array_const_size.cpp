#include <iostream>


void print()
{
    std::cout << __func__ << std::endl;
}

const size_t kLen = 128;

int main( int argc, char* argv[] )
{
    char name[kLen] = {};
    strncpy( name, "aaaaaaaaaa", sizeof(name) );
    std::cout << "name=" << name << std::endl;
    return 0;
}
