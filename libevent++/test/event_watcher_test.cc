
#include "libevent++/exp.h"
#include "test_common.h"

#include "libevent++/libevent_headers.h"
#include "libevent++/libevent_condition.h"
#include "libevent++/libevent_watcher.h"

#include <thread>

namespace
{
    static uint64_t g_timeout_us = 1000000;
    static bool g_event_handler_called = false;
    void Handle(struct event_base* base)
    {
        g_event_handler_called = true;
        //std::cout << __FILE__ << ":" << __LINE__ << " " << __FUNCTION__ << " called ...\n";
        event_base_loopexit(base, 0);
    }

    void MyEventThread(struct event_base* base, evqing::TimedEventWatcher* ev)
    {
        ev->Init();
        ev->Watch(g_timeout_us);
        event_base_loop(base, 0);
    }
}



TEST_UNIT(TimedEventWatcher_test)
{
    struct event_base* base = event_base_new();
    evqing::TimedEventWatcher ev(base, std::tr1::bind(Handle, base));
    std::thread th(MyEventThread, base, &ev);
    uint64_t start = osl::utcmicrosecond();
    th.join();
    uint64_t end = osl::utcmicrosecond();
    H_TEST_ASSERT(end - start >= g_timeout_us);
    H_TEST_ASSERT(g_event_handler_called);
    event_base_free(base);
}

