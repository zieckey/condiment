#ifndef EVENTQING_EVENT_LOOP_THREAD_H_
#define EVENTQING_EVENT_LOOP_THREAD_H_

#include "evpp/inner_pre.h"

#include <boost/thread/thread.hpp>

struct event_base;
struct event;

namespace evpp {

    class EventLoop;
    class _EXPORT_LIBEVENTPP EventLoopThread {
    public:
        typedef xstd::function<void()> Functor;
        EventLoopThread();
        ~EventLoopThread();

        bool Start(bool wait_until_thread_started = false, const Functor& pre = Functor(), const Functor& post = Functor());
        void Stop(bool wait_thread_exit = false);

        void SetName(const std::string& n);
        const std::string& name() const;
        boost::thread::id tid() const;
    public:
        EventLoop* event_loop() const;
        struct event_base *event_base();

        bool IsRunning() const;
        bool IsStopped() const;
    private:

        class Impl;
        xstd::shared_ptr<Impl> impl_;
    };
}

#endif
