#include "evpp/inner_pre.h"

#include <string.h>
#include <boost/static_assert.hpp>

#include "evpp/fd_channel.h"
#include "evpp/libevent_headers.h"

namespace evpp
{
    BOOST_STATIC_ASSERT(FdChannel::kReadable == EV_READ);
    BOOST_STATIC_ASSERT(FdChannel::kWritable == EV_WRITE);

    FdChannel::FdChannel(struct event_base *event_base, int f, bool r, bool w)
        : fd_(f)
    {
        events_ = (w ? kWritable : 0) | (r ? kReadable : 0) | EV_PERSIST;
        event_ = new event;
        memset(event_, 0, sizeof(struct event));
        event_set(event_, fd_, events_, FdChannel::HandlerFn, this);
        event_base_set(evbase_, event_);
    }

    bool FdChannel::Start()
    {
        if (event_add(event_, NULL) != 0) {
            return false;
        }
        return true;
    }

    void FdChannel::Close() {
        if (event_initialized(event_)) {
            event_del(event_);
            delete (event_);
            event_ = NULL;
        }
    }

    void FdChannel::HandlerFn(int fd, short which, void *v) {
        FdChannel *c = (FdChannel*)v;
        c->HandlerFn(fd, which);
    }

    void FdChannel::HandlerFn(int f, short which) {
        if ((which & EV_READ) && read_fn_) {
            read_fn_(Timestamp::Now());
        }

        if ((which & EV_WRITE) && write_fn_) {
            write_fn_();
        }
    }
}

