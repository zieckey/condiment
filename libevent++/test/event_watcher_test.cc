
#include "libevent++/exp.h"
#include "test_common.h"

#include "libevent++/libevent_headers.h"
#include "libevent++/libevent_condition.h"
#include "libevent++/libevent_watcher.h"
//#include <boost/thread.hpp>
#include <thread>

namespace timed
{
    static uint64_t g_timeout_us = 1000000;
    static bool g_event_handler_called = false;
    static void Handle(struct event_base* base)
    {
        g_event_handler_called = true;
        event_base_loopexit(base, 0);
    }

    static void MyEventThread(struct event_base* base, evqing::TimedEventWatcher* ev)
    {
        ev->Init();
        ev->Watch(g_timeout_us);
        event_base_loop(base, 0);
    }
}

TEST_UNIT(TimedEventWatcher_test)
{
    using namespace timed;
    struct event_base* base = event_base_new();
    evqing::TimedEventWatcher ev(base, std::tr1::bind(Handle, base));
    std::thread th(MyEventThread, base, &ev);
    uint64_t start = evqing::utcmicrosecond();
    th.join();
    uint64_t end = evqing::utcmicrosecond();
    H_TEST_ASSERT(end - start >= g_timeout_us);
    H_TEST_ASSERT(g_event_handler_called);
    event_base_free(base);
}

TEST_UNIT(evutil_socketpair_test)
{
    int pipe_[2];
    memset(pipe_, 0, sizeof(pipe_[0] * 2));

    int r = evutil_socketpair(AF_UNIX, SOCK_STREAM, 0, pipe_);
    H_TEST_ASSERT(r >= 0);
}

