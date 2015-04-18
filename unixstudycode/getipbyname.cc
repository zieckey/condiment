
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/file.h>
#include <sys/time.h>
#include <sys/signal.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <errno.h>

#include <string>


std::string getipbyname(const char* target)
{
    struct hostent *hp = NULL;
    struct sockaddr_in *to = NULL;
    //u_char *datap, *packet;
    //char *target, hnamebuf[MAXHOSTNAMELEN];
    struct sockaddr whereto;    /* who to ping */

    std::string ip;

    memset(&whereto, 0, sizeof(struct sockaddr));
    to = (struct sockaddr_in *)&whereto;
    to->sin_family = AF_INET;
    if (inet_aton(target, &to->sin_addr)) {
        ip = target;
        printf("%s:%d ip=%s\n", __FILE__, __LINE__, ip.data());
    }
    else {
        hp = gethostbyname(target);
        if (!hp) {
            (void)fprintf(stderr,
                        "ping: unknown host %s\n", target);
            exit(2);
        }
        to->sin_family = hp->h_addrtype;
        if (hp->h_length > (int)sizeof(to->sin_addr)) {
            hp->h_length = sizeof(to->sin_addr);
        }
        memcpy(&to->sin_addr, hp->h_addr, hp->h_length);
        //(void)strncpy(hnamebuf, hp->h_name, sizeof(hnamebuf) - 1);
        ip = inet_ntoa(to->sin_addr);
        printf("%s:%d hostname=%s\n", __FILE__, __LINE__, hp->h_name);
        printf("%s:%d ip=%s\n", __FILE__, __LINE__, inet_ntoa(to->sin_addr));
    }

    return ip;
}


int main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("Usage: %s hostname\n", argv[0]);
        return (-1);
    }

    const char* target = argv[1];
    
    std::string ip = getipbyname(target);

    printf("%s:%d ip=%s\n", __FILE__, __LINE__, ip.data());
    return 1;
}
