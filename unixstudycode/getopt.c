#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>


void usage()
{
    printf("./getopt -b -f /tmp/a.txt\n");
}

int main( int argc, char* argv[] )
{
    int bflag, ch, fd;
    bflag = 0;
    while ((ch = getopt(argc, argv, "bf:")) != -1) {
        switch (ch) {
            case 'b':
                bflag = 1;
                printf("The parameter of -b is set!\n", optarg);
                break;
            case 'f':
                printf("The parameter of -f=%s\n", optarg);
                break;
            case '?':
            default:
                usage();
        }
    }

    printf("After parsing argument : argc=%d optind=%d\n", argc, optind);

    return 0;
}
