#include <stdio.h>
#include <stdlib.h>
#include <event.h>
#include <evhttp.h>

void generic_request_handler(struct evhttp_request *req, void *arg) {

    fprintf(stdout, "%s called ...\n", __func__);

    struct evbuffer *return_buffer=evbuffer_new();

    evbuffer_add_printf(return_buffer,"welcome");
    evhttp_send_reply(req,HTTP_OK,"Client",return_buffer    );
    evbuffer_free(return_buffer);
}

int main(int argc, char **argv) {
    short http_port =8082;
    const char *http_addr="127.0.0.1";
    struct evhttp *http_serv=NULL;
    struct event_base* base = event_base_new();

    http_serv = evhttp_new(base);
    evhttp_bind_socket(http_serv, http_addr, http_port);
    evhttp_set_gencb(http_serv,generic_request_handler,NULL);
    event_base_dispatch(base);
    return 0;
}

