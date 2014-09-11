/** 
 * Compile with: 
 * gcc -I/usr/local/include -o ss ss.c -L/usr/local/lib -levent 
 */
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <event.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

void write_time(int fd, short event, /*struct event* */ void* arg)
{
    char buf[64] = {};
    struct tm t;
    time_t now;

    time(&now);
    localtime_r(&now, &t);
    asctime_r(&t, buf);

    write(fd, buf, strlen(buf));
    fprintf(stdout, "write time to client, and close this socket\n");
    shutdown(fd, SHUT_RDWR);

    /** malloc by connection_accept*/
    free(arg);
}

void connection_accept(int fd, short event, void *arg)
{
    /* for debugging */
    fprintf(stdout, "%s(): fd = %d, event = %d.\n", __func__, fd, event);

    /* Accept a new connection. */
    struct sockaddr_in s_in;
    socklen_t len = sizeof(s_in);
    int newsock = accept(fd, (struct sockaddr *) &s_in, &len);
    if (newsock < 0) {
        perror("accept");
        return;
    }

    /* Install time server. */
    struct event *ev = malloc(sizeof(struct event)); /** will free by write_time */
    event_set(ev, newsock, (short)EV_WRITE, &write_time, ev);
    event_add(ev, NULL);
}

int main(void)
{
    /* Request socket. */
    int s = socket(PF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        perror("socket");
        exit(1);
    }

    /* bind() */
    struct sockaddr_in s_in;
    bzero(&s_in, sizeof(s_in));
    s_in.sin_family = AF_INET;
    s_in.sin_port = htons(7000);
    s_in.sin_addr.s_addr = INADDR_ANY;
    if (bind(s, (struct sockaddr *) &s_in, (socklen_t)sizeof(s_in)) < 0) {
        perror("bind");
        exit(1);
    }

    /* listen() */
    if (listen(s, 5) < 0) {
        perror("listen");
        exit(1);
    }

    /* Initial libevent. */
    event_init();

    /* Create event. */
    struct event ev;
    event_set(&ev, s, (short)(EV_READ | EV_PERSIST), &connection_accept, &ev);

    /* Add event. */
    event_add(&ev, NULL);

    event_dispatch();

    return 0;
}
