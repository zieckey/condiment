#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
//#include <>

static void sig_quit(int signo)
{
    printf("caught SIGQUIT\n");
    //signal(SIGQUIT, SIG_DFL);//将SIGQUIT的动作设为缺省值
}

int main()
{
    printf("pid=%d\n", getpid());
    sigset_t newmask;
    sigset_t oldmask;
    sigset_t pendmask;

    signal(SIGQUIT, sig_quit);//信号量捕捉函数，捕捉到SIGQUIT，跳转到函数指针sig_quit处执行

    sigemptyset(&newmask);//初始化信号量集
    sigaddset(&newmask, SIGQUIT);//将SIGQUIT添加到信号量集中

    sigprocmask(SIG_BLOCK, &newmask, &oldmask);//将newmask中的SIGQUIT阻塞掉，并保存当前信号屏蔽字到Oldmask

    kill(getpid(), SIGQUIT);
    printf("Send a QUIT sig this pid=%d\n", getpid());
    
    printf("sleeping ...\n");
    sleep (5);//休眠5秒钟，这里做说明:在休眠期间，接收到的SIGQUIT信号都会被阻塞，此时会把这些信息存在内核的队列中，等待休眠结束后，再处理此信号。 
    printf("awaked\n");

    //检查悬而未决的信号
    sigpending(&pendmask);
    if (sigismember(&pendmask, SIGQUIT))//SIGQUIT是悬而未决的。所谓悬而未决，是指SIGQUIT被阻塞还没有被处理
    {
        printf("SIGQUIT pending\n");
    }

    printf("unblocking SIGQUIT\n");
    sigprocmask(SIG_SETMASK, &oldmask, NULL);//恢复被屏蔽的信号SIGQUIT, 在此之后，就可以处理该信号了
    /**开始处理信号，调用信号处理函数*/ 

    printf("exited\n");

    return (0);
}
