#include <stdio.h>

#define CONCAT(x,y)   x##y

int main( int argc, char* argv[] )
{
    double freebsdversion = 6.2;
    printf( "%f\n", freebsdversion );
    CONCAT(freebsd,version) = 7.2;
    printf( "%f\n", freebsdversion );

    return 0;
}
