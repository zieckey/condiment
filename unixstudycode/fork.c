#include <sys/types.h>
#include <unistd.h>

pid_t fork(void);

int main(int argc, char* argv[])
{
    pid_t pid = fork();
    if (pid == 0)
    {
        //child process
        printf("Child process, pid=%d ppid=%d\n", getpid(), getppid());
    }
    else
    {
        printf("Parent process, pid=%d ppid=%d\n", getpid(), getppid());
    }

    return 0;
}
