#include<stdio.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<stdlib.h>

int main()
{
    pid_t child;
    int i;
    child = fork();
    if(child < 0){
        printf("create failed!\n");
        return (1);
    }
    else if (0 == child){
        printf("this is the child process pid= %d\n",getpid());
        for(i = 0;i<5;i++){
            printf("this is the child process print %d !\n",i+1);
        }
        printf("the child end\n");
    }
    else{
        printf("this is the father process,ppid=%d\n",getppid());
        printf("father wait the child end\n");
        wait(&child);
        printf("father end\n");
    }

    return 0;
}
