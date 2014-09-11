#include <sys/time.h>
#include <event.h>   
#include <stdio.h>  

struct event ev;
struct timeval tv;

int iCount = 1;

void time_cb(int fd, short event, void *argc)
{
    printf("timer wakeup fd=%d event=%d count=%d \n", fd, event, iCount++ );
    if(iCount > 5)
    {
        event_del(&ev);
    }
    else
    {
        event_add(&ev, &tv);
    }

}

int main()
{
    struct event_base *base = event_init();

    tv.tv_sec = 1;
    tv.tv_usec = 0;

    evtimer_set(&ev, time_cb, NULL);
    event_add(&ev, &tv);
    event_base_set(base, &ev);
    event_base_dispatch(base);
    
    return 0;
}
