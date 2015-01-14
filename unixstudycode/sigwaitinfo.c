#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <sys/time.h>

int main(int argc, char *argv[])
{
    sigset_t block;
    struct itimerval itv;

    sigemptyset(&block);
    sigaddset(&block, SIGALRM);
    sigprocmask(SIG_BLOCK, &block, NULL);

    itv.it_interval.tv_sec = 2;
    itv.it_interval.tv_usec = 0;
    itv.it_value = itv.it_interval;
    setitimer(ITIMER_REAL, &itv, NULL);

    while (1) {
        printf("%lu\n", (unsigned long)time(NULL));
        sigwaitinfo(&block, NULL);
    }

    return 0;
}
