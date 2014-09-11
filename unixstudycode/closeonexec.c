#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if (argc > 1) {
        int flags = fcntl(STDOUT_FILENO, F_GETFD);
        if (flags == -1) {
            perror("fctnl(STDOUT_FILENO, F_GETFD) ERROR:");
            return -1;
        }

        flags |= FD_CLOEXEC;
        if (fcntl(STDOUT_FILENO, F_SETFD, flags) == -1) {
            perror("fctnl(STDOUT_FILENO, F_SETFD) ERROR:");
            return -1;
        }
    }

    execlp("ls", "ls", "-l", argv[0], (char*)NULL);
    return 0;
}
