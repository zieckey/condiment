
#include "evpp/exp.h"
#include "test_common.h"

#include "evpp/libevent_headers.h"
#include "evpp/libevent_watcher.h"
#include "evpp/event_loop.h"

#include <boost/thread.hpp>

namespace evloop
{
    static boost::shared_ptr<evpp::EventLoop> loop;
    static double delay_ms = 1000.0;
    static bool g_event_handler_called = false;
    static void Handle(evpp::EventLoop* evloop) {
        g_event_handler_called = true;
        evloop->Stop();
    }

    static void MyEventThread() {
        loop = boost::shared_ptr<evpp::EventLoop>(new evpp::EventLoop);
        loop->Run();
    }
}

TEST_UNIT(EventLoop_test)
{
    using namespace evloop;
    boost::thread th(MyEventThread);
    uint64_t start = evpp::utcmicrosecond();
    usleep(delay_ms*1000);
    //loop->RunAfter(delay_ms, xstd::bind(&Handle, loop.get(), start));
    Handle(loop.get());
    th.join();
    uint64_t end = evpp::utcmicrosecond();
    H_TEST_ASSERT(end - start >= delay_ms*1000);
    H_TEST_ASSERT(g_event_handler_called);
}

