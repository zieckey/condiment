
/**
 * ./redirectory 
 */
#include<stdio.h>
int main( int argc, char* argv[] )
{
    printf("Under bash, Usage: %s 1>>std.txt 2>>std.txt\n", argv[0] );
    printf("stdout\n");
    fprintf(stderr,"stderr\n" );
    return 0;
}
