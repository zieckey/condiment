#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
int main(int argc,char **argv)
{
    int z;
    int s[2];
    const char *msgp;
    int mlen;
    char buf[80] = {0};
    pid_t chpid;
    //create a pair of TCP socket
    z=socketpair(AF_UNIX,SOCK_STREAM,0,s);
    if(z==-1)
    {
        printf("socketpair error:%s\n",strerror(errno));
        exit(1);
    }
    //now fork to two processes
    if((chpid=fork())==-1)
    {
        //failed
        printf("fork error:%s\n",strerror(errno));
        exit(1);
    }
    else if(chpid==0) 
    {
        //the child process(client)
        char rxbuf[80] = {0}; //receive buffer
        printf("parent id:%d\n",getppid());
        close(s[0]);//server uses s[1],so there is no usage.
        s[0]=-1;//forget it
        msgp="%A %d-%b-%Y %l:%M %p";
        mlen=strlen(msgp);
        printf("child sending request: '%s'\n",msgp);
        fflush(stdout);
        //write a quest to server
        z=write(s[1],msgp,mlen);
        if(z<0)
        {
            printf("write error:%s\n",strerror(errno));
            exit(1);
        }
        //close the write side of the socket
        if(shutdown(s[1],SHUT_WR)==-1)
        {
            printf("shutdown ShUT_WR error:%s\n",strerror(errno));
            exit(1);
        }
        //receive the reply from server
        z=read(s[1],rxbuf,sizeof(rxbuf));
        if(z<0)
        {
            printf("read from server error:%s\n",strerror(errno));
            exit(1);
        }
        //put a null byte at the end of what we received from the server
        rxbuf[z]=0;
        //report the result
        printf("child mypid=%d Server returned:%s\n", getpid(), rxbuf);
        close(s[1]);
    }
    else 
    {
        //the parent process (server)
        int status;//child process terminated status
        char txbuf[80] = {0};
        time_t td;
        printf("mypid=%d Child pid:%d\n", getpid(), chpid);
        fflush(stdout);
        close(s[1]);
        s[1]=-1;
        //wait for a request from server
        z=read(s[0],buf,sizeof(buf));
        printf("parent mypid=%d read:%s\n", getpid(), buf);
        if(z<0)
        {
            printf("read error:%s\n",strerror(errno));
            exit(1);
        }
        //put a null byte at the end of the message we received from client
        buf[z]=0;
        //get current time
        time(&td);
        //format date and time
        strftime(txbuf,sizeof(txbuf),buf,localtime(&td));
        //send back the response to client
        z=write(s[0],txbuf,strlen(txbuf));
        if(z<0)
        {
            printf("write error:%s\n",strerror(errno));
            exit(1);
        }
        //close our end of the socket
        close(s[0]);
        //wait for the child process to exit
        waitpid(chpid,&status,0);
    }
    return 0;
}

