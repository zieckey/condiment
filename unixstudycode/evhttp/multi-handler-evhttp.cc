#include <sys/types.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

#include <event.h>
#include <evhttp.h>

void
root_handler(struct evhttp_request *req, void *arg)
{
    struct evbuffer *buf;

    buf = evbuffer_new();
    if (buf == NULL)
      err(1, "failed to create response buffer");
    evbuffer_add_printf(buf, "Hello World!\n");
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
    evbuffer_free(buf);
}

void
generic_handler(struct evhttp_request *req, void *arg)
{
    struct evbuffer *buf;

    buf = evbuffer_new();
    if (buf == NULL)
      err(1, "failed to create response buffer");
    evbuffer_add_printf(buf, "Requested: %s\n", evhttp_request_uri(req));
    evhttp_send_reply(req, HTTP_OK, "OK", buf);
    evbuffer_free(buf);
}

int
main(int argc, char **argv)
{
    struct evhttp *httpd;

    event_init();

    httpd = evhttp_start("127.0.0.1", 8088);

    if (!httpd) {
        printf("server not started!\n");
        return 0;
    }

    /* Set a callback for requests to "/". */
    evhttp_set_cb(httpd, "/", root_handler, NULL);

    /* Set a callback for all other requests. */
    evhttp_set_gencb(httpd, generic_handler, NULL);

    event_dispatch();

    /* Not reached in this code as it is now. */

    evhttp_free(httpd);

    return 0;
}
