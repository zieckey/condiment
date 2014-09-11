#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

int main( int argc, char* argv[] )
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage:%s [the file name to be created or opened]\n");
        return -1;
    }
    const char* path = argv[1];
    int fd = open(path, O_CREAT|O_RDWR);
    if (fd < 0)
    {
        int err = errno;
        fprintf(stderr, "open file error %s : %d %s\n", path, err, strerror(err));
        return -2;
    }
    int ret = fchmod(fd, 0755);
    if (ret < 0)
    {
        int err = errno;
        fprintf(stderr, "fchmod file error %s : %d %s\n", path, err, strerror(err));
        close(fd);
        return -2;
    }
    const char* data = "1\t2";
    int n = write(fd, data, strlen(data));
    if (n < 0)
    {
        int err = errno;
        fprintf(stderr, "write file error : %d %s\n", err, strerror(err));
    }
    close(fd);
    return 0;
}


