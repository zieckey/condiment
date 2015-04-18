#include "evqing/include/inner_pre.h"

#include "evqing/include/http_context.h"
#include "evqing/include/libevent_headers.h"

namespace evqing
{
    const char* HTTPContext::remote_ip() const
    {
        return req->remote_host;
    }

    const char* HTTPContext::original_uri() const
    {
        return req->uri;
    }
}





