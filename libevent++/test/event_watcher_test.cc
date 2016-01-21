
#include "src/exp.h"
#include "test_common.h"

#include "src/libevent_headers.h"
#include "src/libevent_watcher.h"
#include <boost/thread.hpp>

namespace timed
{
    static uint64_t g_timeout_us = 1000000;
    static bool g_event_handler_called = false;
    static void Handle(struct event_base* base)
    {
        g_event_handler_called = true;
        event_base_loopexit(base, 0);
    }

    static void MyEventThread(struct event_base* base, evpp::TimerEventWatcher* ev)
    {
        ev->Init();
        ev->Watch(g_timeout_us);
        event_base_loop(base, 0);
    }
}

TEST_UNIT(TimerEventWatcher_test)
{
    using namespace timed;
    struct event_base* base = event_base_new();
    evpp::TimerEventWatcher ev(base, std::tr1::bind(Handle, base));
    boost::thread th(MyEventThread, base, &ev);
    uint64_t start = evpp::utcmicrosecond();
    th.join();
    uint64_t end = evpp::utcmicrosecond();
    H_TEST_ASSERT(end - start >= g_timeout_us);
    H_TEST_ASSERT(g_event_handler_called);
    event_base_free(base);
}

TEST_UNIT(evutil_socketpair_test)
{
    int sockpair[2];
    memset(sockpair, 0, sizeof(sockpair[0] * 2));

    int r = evutil_socketpair(AF_UNIX, SOCK_STREAM, 0, sockpair);
    H_TEST_ASSERT(r >= 0);
    EVUTIL_CLOSESOCKET(sockpair[0]);
    EVUTIL_CLOSESOCKET(sockpair[1]);
}

