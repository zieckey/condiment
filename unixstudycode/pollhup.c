#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <poll.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void usage(char* argv[]) {
    printf("usage : %s c|w|r\n", argv[0]);
    printf("e.g. : %s wc\n", argv[0]);
    printf("e.g. : %s rc\n", argv[0]);
    printf("c : close\n");
    printf("w : shutdown write\n");
    printf("r : shutdown read\n");
    printf("-h : show this help message\n");
}

/**
 * 1. 关闭读：不会触发对方任何事件。
 * 2. 关闭写：会触发对方 POLLIN 事件，对方然后调用 read/recv 返回 0.
 * 3. 关闭读和写：会触发对方 POLLIN|POLLHUP 事件
 * 4. 如果先后关闭读写(或写读)，后面的那个动作就会触发对方 POLLIN|POLLHUP 事件，相当于只调用一次 close
 */

int main(int argc, char* argv[])
{
    int i = 0;
    int p[2];
    struct pollfd ufd;

    if (argc != 2 || strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0 ) {
        usage(argv);
        return 0;
    }

    if (socketpair(AF_UNIX, SOCK_STREAM, 0, p) < 0) {
        perror("socketpair");
        return EXIT_FAILURE;
    }

    //write(p[1], "aa", 2);

    for (i = 0; i < strlen(argv[1]); ++i) {

        if (argv[1][i] == 'w') {
            printf("shutdown-write:\t");
            if (shutdown(p[1], SHUT_WR) < 0) { /* close the fd writing*/
                perror("shutdown");
                return EXIT_FAILURE;
            }
        }

        if (argv[1][i] == 'r') {
            printf("shutdown-read:\t");
            if (shutdown(p[1], SHUT_RD) < 0) { /* close the fd reading*/
                perror("shutdown");
                return EXIT_FAILURE;
            }
        }

        if (argv[1][i] == 'c') {
            printf("close:\t");
            if (close(p[1]) < 0) { /* close the fd */
                perror("close");
                return EXIT_FAILURE;
            }
        }

        memset(&ufd, 0, sizeof ufd);
        ufd.fd = p[0]; /* poll the read fd after the write fd is closed */
        ufd.events = POLLIN;
        if (poll(&ufd, 1, 1000) < 0) {
            perror("poll");
            return EXIT_FAILURE;
        }

        switch(ufd.revents & (POLLIN|POLLHUP)) {
            case POLLIN: printf("POLLIN\n"); break;
            case POLLHUP: printf("POLLHUP\n"); break;
            case POLLIN|POLLHUP: printf("POLLIN|POLLHUP\n"); break;
            case POLLERR: printf("POLLERR\n"); break;
            default: printf("%#x\n", (unsigned)ufd.revents); break;
        }
    }

    return EXIT_SUCCESS;
}
