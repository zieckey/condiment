#include <sys/time.h>
#include <stdio.h>

int main()
{
    struct timeval tv;
    printf("begin to call gettimeofday\n");
    gettimeofday(&tv, NULL);//to verify gettimeofday is not a system call
    printf("after call gettimeofday, current time=%ld\n", (unsigned int)tv.tv_sec);
    return 0;
}
