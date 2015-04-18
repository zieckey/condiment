/*
* 获取某个域名的ip地址,以列表形式打印。
*
* 作者: nswish
* 2012-01-30
*
* 在如下环境编译通过：
* Kubuntu 11.1
* gcc version 4.6.1
*/
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    struct addrinfo * res, *pt;
    struct sockaddr_in *sinp;
    const char *addr;
    char abuf[INET_ADDRSTRLEN];
    int succ=0,i=0;

    if (argc == 1)
    {
        printf("USAGE: %s <server name>\nFor example:\n    %s www.google.com\n", argv[0], argv[0]);
        exit(1);
    }

    succ = getaddrinfo(argv[1], NULL, NULL, &res);
    if(succ != 0)
    {
        printf("Can't get address info! error code = %d", succ);
        exit(succ);
    }

    for(pt=res, i=0; pt != NULL; pt=pt->ai_next, i++){
        sinp = (struct sockaddr_in *)pt->ai_addr;
        //addr = (const char *)inet_ntop(AF_INET, &sinp->sin_addr, abuf, INET_ADDRSTRLEN);
        addr = inet_ntoa(sinp->sin_addr); //BUGGGGGGGGGGGG!!!!
        printf("%d IP=%s\n", i, addr);
    }

return 0;
}
