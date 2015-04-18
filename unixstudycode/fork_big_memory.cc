#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    const size_t length = 1024*1024*100;
    long * p = new long[length];

    for (size_t i = 0; i < length; ++i) {
        p[i] = i;
        //i += i;
    }

    vfork();
    vfork();
    vfork();

    long sum = 0;
    for (int i = 0; i < length;) {
        sum += p[i];
        usleep(10000);
    }

    return 0;
}
