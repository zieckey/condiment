#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

int main( int argc, char* argv[] )
{
    if ( argc != 2 )
    {
        fprintf( stderr, "usage: %s the-path-to-mkdir\n", argv[0] );
        return -1;
    }

    mode_t mt = 0755;
    int ret = mkdir( argv[1], mt );

    if ( ret != 0 )
    {
        perror( "mkdir failed : " );
        fprintf( stderr, "mkdir ret=%d\n", ret );
        return -2;
    }

    return 0;
}
