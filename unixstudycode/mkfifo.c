#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    char buf[256] = {};
    int fd;
    unlink( "zieckey_fifo" );

    mode_t md = 055;
    mkfifo( "zieckey_fifo", md );

    if ( fork() > 0 )
    {
        char s[] = "Hello!\n";
        fd = open( "zieckey_fifo", O_WRONLY );
        write( fd, s, sizeof(s) );
        //close( fd );
    }
    else
    {
        fd = open( "zieckey_fifo", O_RDONLY );
        read( fd, buf, sizeof(buf) );
        printf("The message from the pipe is:%s\n", buf );
        //close( fd );
    }

    return 0;
}
