#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void checkset();

int main()
{
    sigset_t blockset;
    sigemptyset(&blockset);
    sigaddset(&blockset,SIGINT);
    sigaddset(&blockset,SIGTSTP);

    checkset();

    sigprocmask(SIG_SETMASK,&blockset,NULL);
    checkset();

    sigaddset(&blockset,SIGTERM);
    sigprocmask(SIG_BLOCK,&blockset,NULL);
    checkset();

    sigdelset(&blockset,SIGTERM);
    sigprocmask(SIG_UNBLOCK,&blockset,NULL);
    checkset();
    return 0;
}

void checkset()
{

    sigset_t set;
    printf("checkset start ...\n");

    // get the current sigmask of the process
    if(sigprocmask(0,NULL,&set)<0)
    {
        printf("checkset sigprocmask error!!\n");
        exit(0);
    }

    if(sigismember(&set,SIGINT))
      printf("\tsigint\n");
    if(sigismember(&set,SIGTSTP))
      printf("\tsigtstp\n");
    if(sigismember(&set,SIGTERM))
      printf("\tsigterm\n");

    printf("checkset end\n\n");
}

/* output :

checkset start ...
checkset end

checkset start ...
        sigint
        sigtstp
checkset end

checkset start ...
        sigint
        sigtstp
        sigterm
checkset end

checkset start ...
        sigterm
checkset end
*/


