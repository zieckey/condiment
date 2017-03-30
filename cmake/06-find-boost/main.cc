//main.cc
#include <stdio.h>
#include <boost/lexical_cast.hpp>

int main()
{
    int x = boost::lexical_cast<int>("100");
    int y = boost::lexical_cast<int>("200");
    printf("x+y=%d\n", x+y);
    return 0;
}
