/* 
 * File:   Client.c
 * Author: root
 *
 * Created on 2012年6月20日, 下午1:30
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVERIP "192.168.0.23"
#define SERVERPORT 12345
#define MAXBUFFER 256

int main(int argc, char** argv)
{  
    int clientFd,ret;
    struct sockaddr_in serveraddr;
    char buf[MAXBUFFER];
    clientFd=socket(AF_INET,SOCK_STREAM,0);//创建socket
    if(clientFd < 0)
    {
        printf("socket error:%s\n",strerror(errno));
        exit(-1);
    }
    bzero(&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family=AF_INET;
    serveraddr.sin_port=htons(SERVERPORT);
    inet_pton(AF_INET,SERVERIP,&serveraddr.sin_addr);
    ret=connect(clientFd,(struct sockaddr *)&serveraddr,sizeof(serveraddr));//连接到服务器
    if(ret!=0)
    {
        close(clientFd);
        printf("connect error:%s\n",strerror(errno));
        exit(-1);
    }
    while(1)
    {
        bzero(buf,sizeof(buf));
        scanf("%s",buf);
        write(clientFd,buf,sizeof(buf));//写数据
        bzero(buf,sizeof(buf));
        read(clientFd,buf,sizeof(buf));//读数据
        printf("echo:%s\n",buf);
    }
    close(clientFd);
    return (EXIT_SUCCESS);
}

