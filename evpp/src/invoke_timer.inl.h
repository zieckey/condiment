#ifndef LIBEVENTPP_INVOKER_TIMER_H_
#define LIBEVENTPP_INVOKER_TIMER_H_

#include "evpp/inner_pre.h"

#include "evpp/event_loop.h"
#include "evpp/libevent_watcher.h"

#include <boost/enable_shared_from_this.hpp>

namespace evpp {
    class InvokeTimer : public boost::enable_shared_from_this<InvokeTimer>, public boost::noncopyable
    {
    public:
        typedef xstd::function<void()> Functor;

        static boost::shared_ptr<InvokeTimer> Create(EventLoop* evloop, double timeout_ms, const Functor& f) {
            boost::shared_ptr<InvokeTimer> it(new InvokeTimer(evloop, timeout_ms, f));
            return it;
        }

        ~InvokeTimer() {
            if (watcher_) {
                delete watcher_;
                watcher_ = NULL;
            }
        }

        void Start() {
            watcher_ = new TimerEventWatcher(loop_->event_base(), xstd::bind(&InvokeTimer::OnTimeout, this));
            watcher_->set_cancel_callback(xstd::bind(&InvokeTimer::OnCanceled, this));
            watcher_->Init();

            loop_->RunInLoop(xstd::bind(&InvokeTimer::AsyncWait, shared_from_this(), timeout_us_));
        }

        void Cancel() {
            loop_->RunInLoop(xstd::bind(&TimerEventWatcher::Cancel, watcher_));
        }

        EventLoop* loop() {
            return loop_;
        }

    private:
        InvokeTimer(EventLoop* evloop, double timeout_ms, const Functor& f)
            : loop_(evloop), timeout_us_((uint64_t)(timeout_ms * 1000)), functor_(f), watcher_(NULL)
        {}

        void AsyncWait(uint64_t timeout_us) {
            watcher_->AsyncWait(timeout_us);
        }

        void OnCanceled()
        {
        }

        void OnTimeout()
        {
            functor_();
        }



    private:
        EventLoop* loop_;
        uint64_t timeout_us_;
        Functor functor_;
        TimerEventWatcher* watcher_;
    };
}
#endif

