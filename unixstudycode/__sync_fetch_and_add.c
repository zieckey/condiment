#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

static int count = 0;

const int thread_count = 20;
int loop_count   = 20000;

void *test_func(void *arg)
{
    int i=0;
    for(i=0;i<loop_count;++i){
        __sync_fetch_and_add(&count,1);
    }
    return NULL;
}

int main(int argc, const char *argv[])
{
    pthread_t id[thread_count];
    int i = 0;

    for(i=0;i<thread_count;++i){
        pthread_create(&id[i],NULL,test_func,NULL);
    }

    for(i=0;i<thread_count;++i){
        pthread_join(id[i],NULL);
    }

    if (thread_count * loop_count == count)
    {
        printf("__sync_fetch_and_add test pass!\n");
    }
    else
    {
        printf("__sync_fetch_and_add test failed, count=%d!!!!\n", count);
    }
    printf("%d\n",count);

    __sync_lock_release(&count);
    printf("after __sync_lock_release , count=%d\n", count);
    return 0;
}
