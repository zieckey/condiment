#include "evpp/inner_pre.h"

#include "evpp/libevent_headers.h"
#include "evpp/libevent_watcher.h"
#include "evpp/event_loop.h"
#include "invoke_timer.inl.h"

#include <boost/thread/lock_guard.hpp>


namespace evpp
{
    EventLoop::EventLoop()
    {
        event_base_ = event_base_new();

        Init();
    }

    EventLoop::EventLoop(struct event_base *base)
        : event_base_(base)
    {
        Init();
    }

    EventLoop::~EventLoop()
    {
        if (event_base_ != NULL) {
            event_base_free(event_base_);
        }
    }

    void EventLoop::Init(void)
    {
        calling_pending_functors_ = false;
        watcher_.reset(new PipeEventWatcher(event_base_, xstd::bind(&EventLoop::DoPendingFunctors, this)));
        watcher_->Init();
        watcher_->Watch(0);
    }


    void EventLoop::Run() {

        tid_ = boost::this_thread::get_id();

        //int rc = event_base_dispatch(event_base_);
        int rc = event_base_loop(event_base_, 0);
        DoAfterLoopFunctors();
        if (rc == 1) {
            LOG_ERROR << "event_base_dispatch error: no event registered";
            return;
        } else if (rc == -1) {
            LOG_FATAL << "event_base_dispatch error";
            return;
        }
        LOG_INFO << "EventLoop stopped, tid: " << boost::this_thread::get_id();
    }

    void EventLoop::Stop() {
        RunInLoop(xstd::bind(&EventLoop::StopInLoop, this));
    }

    void EventLoop::StopInLoop() {
        LOG_INFO << "EventLoop is stopping now, tid: " << boost::this_thread::get_id();
        for (;;) {
            DoPendingFunctors();

            boost::lock_guard<boost::mutex> lock(mutex_);
            if (pending_functors_.empty()) {
                break;
            }
        }

        timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = 500 * 1000; //500ms
        event_base_loopexit(event_base_, &tv);
    }
    void EventLoop::AddAfterLoopFunctor(const Functor& cb) {
        boost::lock_guard<boost::mutex> lock(mutex_);
        after_loop_functors_.push_back(cb);
    }

    void EventLoop::DoAfterLoopFunctors() {
        std::vector<Functor> functors;
        {
            boost::lock_guard<boost::mutex> lock(mutex_);
            functors.swap(after_loop_functors_);
        }
        for (size_t i = 0; i < functors.size(); ++i)
        {
            functors[i]();
        }

    }

    void EventLoop::AfterFork() {
        int rc = event_reinit(event_base_);
        assert(rc == 0);
        if (rc != 0)
        {
            fprintf(stderr, "event_reinit failed!\n");
            abort();
        }
        //CHECK(rc == 0) << "event_reinit" << rc;
    }

    void EventLoop::RunAfter(double delay_ms, const Functor& f)
    {
        boost::shared_ptr<InvokeTimer> t = InvokeTimer::Create(this, delay_ms, f);
        t->Start();
    }

    void EventLoop::RunInLoop(const Functor& functor) {
        boost::thread::id cur_tid = boost::this_thread::get_id();
        if (cur_tid == tid_) {
            functor();
        } else {
            QueueInLoop(functor);
        }
    }

    void EventLoop::QueueInLoop(const Functor& cb) {
        {
            boost::lock_guard<boost::mutex> lock(mutex_);
            pending_functors_.push_back(cb);
        }

        if (calling_pending_functors_ || boost::this_thread::get_id() != tid_)
        {
            watcher_->Notify();
        }
    }

    void EventLoop::DoPendingFunctors()
    {
        std::vector<Functor> functors;
        calling_pending_functors_ = true;

        {
            boost::lock_guard<boost::mutex> lock(mutex_);
            functors.swap(pending_functors_);
        }

        for (size_t i = 0; i < functors.size(); ++i)
        {
            functors[i]();
        }
        calling_pending_functors_ = false;
    }
}
