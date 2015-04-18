
#include "libevent++/exp.h"
#include "test_common.h"

#include "libevent++/libevent_headers.h"
#include "libevent++/libevent_condition.h"
//#include <boost/thread.hpp>
#include <thread>

namespace
{
    static bool g_event_handler_called = false;
    static void Handle(struct event_base* base)
    {
        g_event_handler_called = true;
        event_base_loopexit(base, 0);
    }

    static void MyEventThread(struct event_base* base, evqing::Condition* ev)
    {
        if (ev->Init(base))
        {
            ev->Watch((uint64_t)0);
        }
        event_base_loop(base, 0);
    }
}

TEST_UNIT(http_Event_test)
{
    struct event_base* base = event_base_new();
    evqing::Condition ev(std::tr1::bind(Handle, base));
    std::thread th(MyEventThread, base, &ev);
    ::usleep(1000 * 100);
    ev.Notify();
    th.join();
    event_base_free(base);
    H_TEST_ASSERT(g_event_handler_called == true);
}

