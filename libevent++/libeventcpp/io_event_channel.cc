#include "inner_pre.h"

#include <string.h>
#include <boost/static_assert.hpp>
#include "io_event_channel.h"
#include "libevent_headers.h"

namespace evpp
{
    BOOST_STATIC_ASSERT(IOEventChannel::kReadable == EV_READ);
    BOOST_STATIC_ASSERT(IOEventChannel::kWritable == EV_WRITE);

    IOEventChannel::IOEventChannel(struct event_base *event_base,
        int fd, bool r, bool w)
        : events_((w?kWritable:0) | (r?kReadable:0) | EV_PERSIST), fd_(fd)
    {
        event_ = new event;
        memset(event_, 0, sizeof(struct event));
        event_set(event_, fd_, events_,
            IOEventChannel::HandlerFn, this);
        event_base_set(evbase_, event_);
    }

    bool IOEventChannel::Start()
    {
        if (event_add(event_, NULL) != 0) {
            return false;
        }
        return true;
    }

    void IOEventChannel::Close() {
        if (event_initialized(event_)) {
            event_del(event_);
            delete (event_);
            event_ = NULL;
        }
    }

    void IOEventChannel::HandlerFn(int fd, short which, void *v) {
        IOEventChannel *e = (IOEventChannel*)v;
        e->HandlerFn(fd, which);
    }

    void IOEventChannel::HandlerFn(int fd, short which) {
        if ((which & EV_READ) && read_cb_)
        {
            read_cb_(Timestamp::Now());
        }

        if ((which & EV_WRITE) && write_cb_)
        {
            write_cb_();
        }
    }
}

