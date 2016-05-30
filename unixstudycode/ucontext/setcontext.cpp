#include <iostream>
#include <stdlib.h>
#include <ucontext.h>
#include <sys/time.h>
#include <boost/progress.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <pthread.h>
#define MAX 10000*1000
void v1()
{
    boost::progress_timer pt;
    ucontext_t context;
    volatile int i = 0;
    getcontext(&context);
    i++;
    if(i <= MAX)
        setcontext(&context);
}

void v2()
{
    boost::progress_timer pt;
    boost::mutex m;
    for(int i = 0; i <=MAX; ++i)
    {
        m.lock();
        m.unlock();
    }
}
void v3()
{
    boost::progress_timer pt;
    pthread_mutex_t m;
    pthread_mutex_init(&m, NULL);
    for(int i = 0; i <=MAX; ++i)
    {
        pthread_mutex_lock(&m);
        pthread_mutex_unlock(&m);
    }
}

int main(int argc, char **argv) {
    
    v1();
    v2();
    v3();
    //std::cout<<pt.elapsed()<<std::endl;
    return 0;
}

