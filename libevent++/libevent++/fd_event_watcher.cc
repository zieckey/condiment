#include "inner_pre.h"

#include <string.h>
#include <boost/static_assert.hpp>
#include "fd_event_watcher.h"
#include "libevent_headers.h"

namespace evpp
{
    BOOST_STATIC_ASSERT(FdEventWatcher::kReadable == EV_READ);
    BOOST_STATIC_ASSERT(FdEventWatcher::kWritable == EV_WRITE);

    FdEventWatcher::FdEventWatcher(struct event_base *event_base,
        int fd)
        : EventWatcher(event_base, Handler()), fd_(fd)
    {
    }

    bool FdEventWatcher::DoInit() {
        event_set(event_, fd_, EV_READ | EV_WRITE | EV_PERSIST,
            FdEventWatcher::HandlerFn, this);
        return true;
    }

    void FdEventWatcher::DoClose() {

    }

    void FdEventWatcher::HandlerFn(int fd, short which, void *v) {
        FdEventWatcher *e = (FdEventWatcher*)v;
        e->HandlerFn(fd, which);
    }

    void FdEventWatcher::HandlerFn(int fd, short which) {
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

