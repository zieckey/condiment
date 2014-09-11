#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>




int main( int argc, char **argv )
{
    mode_t mode = 0666;
    if ( argc !=2 )
    {
        printf( "Usage:[%s] fifo_filename\n", argv[0] );
        return -1;
    }

    if (mkfifo( argv[1], mode)<0 )
    {
        perror( "mkfifo" );
        return -1;
    }

    char buf[1024] = {};
    int fd = open( argv[1], O_RDONLY );
    read( fd, buf, sizeof(buf) );/* 这里会阻塞住，请在另一shell里输入 echo "aaa" > argv[1]  */
    printf("The message from the pipe is:%s\n", buf );
    close( fd );

    return 0;
}   

