#include "inner_pre.h"

#include <string.h>
#include <boost/static_assert.hpp>

#include "libevent_watcher.h"
#include "libevent_headers.h"

namespace evpp
{

    EventWatcher::EventWatcher(struct event_base* evbase, const Handler& handler)
        : evbase_(evbase), handler_(handler)
    {
        event_ = new event;//(struct event*)malloc(sizeof(struct event));
        memset(event_, 0, sizeof(struct event));
    }

    EventWatcher::~EventWatcher() {
        Cancel();
        Close();
        if (event_)
        {
            delete (event_);
            event_ = 0;
        }
    }

    bool EventWatcher::Init() {
        if (!DoInit()) {
            goto failed;
        }
        event_base_set(evbase_, event_);
        return true;

    failed:
        Close();
        return false;
    }


    void EventWatcher::Close() {
        DoClose();
    }

    bool EventWatcher::Watch(uint64_t timeout_us /*= 0*/)
    {
        struct timeval tv;
        struct timeval* timeoutval = NULL;

        if (timeout_us > 0)
        {
            tv.tv_sec = (long)timeout_us / 1000000;
            tv.tv_usec = (long)timeout_us % 1000000;
            timeoutval = &tv;
        }
        
        if (event_add(event_, timeoutval) != 0) {
            return false;
        }

        return true;
    }

    void EventWatcher::Cancel() {
        if (event_initialized(event_)) {
            event_del(event_);
            event_ = NULL;
        }
    }
    
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    
    PipedEventWatcher::PipedEventWatcher(struct event_base *event_base,
        const Handler& handler)
        : EventWatcher(event_base, handler)
    {
        memset(pipe_, 0, sizeof(pipe_[0] * 2));
    }

    bool PipedEventWatcher::DoInit() {
        assert(pipe_[0] == 0);

        if (evutil_socketpair(AF_UNIX, SOCK_STREAM, 0, pipe_) < 0) {
            LOG(ERROR) << "create socketpair ERROR";
            goto failed;
        }

        if (evutil_make_socket_nonblocking(pipe_[0]) < 0 ||
            evutil_make_socket_nonblocking(pipe_[1]) < 0) {
            goto failed;
        }

        event_set(event_, pipe_[1], EV_READ | EV_PERSIST,
            PipedEventWatcher::HandlerFn, this);
        return true;
    failed:
        Close();
        return false;
    }

    void PipedEventWatcher::DoClose() {
        if (pipe_[0] > 0) {
            EVUTIL_CLOSESOCKET(pipe_[0]);
            EVUTIL_CLOSESOCKET(pipe_[1]);
            memset(pipe_, 0, sizeof(pipe_[0]) * 2);
        }
    }

    void PipedEventWatcher::HandlerFn(int fd, short which, void *v) {
        PipedEventWatcher *e = (PipedEventWatcher*)v;
        char buf[128];
        int n = 0;
        if ((n = recv(e->pipe_[1], buf, sizeof(buf), 0)) > 0) {
            try {
                e->handler_();
            }
            catch (...) {}
        }
    }

    void PipedEventWatcher::Notify() {
        char buf[1] = {};
        if (send(pipe_[0], buf, sizeof(buf), 0) < 0) {
            return;
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    TimedEventWatcher::TimedEventWatcher(struct event_base *event_base,
        const Handler& handler)
        : EventWatcher(event_base, handler)
    {
    }

    bool TimedEventWatcher::DoInit() {
        event_set(event_, -1, 0, TimedEventWatcher::HandlerFn, this);
        return true;
    }

    void TimedEventWatcher::HandlerFn(int fd, short which, void *v) {
        TimedEventWatcher *h = (TimedEventWatcher*)v;
        h->handler_();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
#ifdef H_OS_LINUX
    SignalEventWatcher::SignalEventWatcher(int signo, struct event_base *event_base,
        const Handler& handler)
        : EventWatcher(event_base, handler)
        , signo_(signo)
    {
        assert(signo_);
    }

    bool SignalEventWatcher::DoInit() {
        assert(signo_);
        signal_set(event_, signo_, SignalEventWatcher::HandlerFn, this);
        return true;
    }

    void SignalEventWatcher::HandlerFn(int sn, short which, void *v) {
        SignalEventWatcher *h = (SignalEventWatcher*)v;
        h->handler_();
    }
#endif
}

