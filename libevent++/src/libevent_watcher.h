
#ifndef LIBEVENTPP_EVENT_WATCHER_H_
#define LIBEVENTPP_EVENT_WATCHER_H_

#include "inner_pre.h"

#ifdef H_OS_WINDOWS
#include <functional>
#else
#include <tr1/functional>
#endif

struct event;
struct event_base;

namespace evpp {


    class _EXPORT_LIBEVENTPP EventWatcher {
    public:
        typedef std::tr1::function<void()> Handler;

        virtual ~EventWatcher();

        bool Init();

        bool Watch(uint64_t timeout_us /*= 0*/);

        void Cancel();

    protected:
        void set_evbase(struct event_base* evbase) {
            evbase_ = evbase;
        }

    protected:
        EventWatcher(struct event_base* evbase, const Handler& handler);

        void Close();

        virtual bool DoInit() = 0;
        virtual void DoClose() {}

    protected:
        static void HandleEvent(int, short, void*);

        struct event*      event_;
        struct event_base* evbase_;
        Handler handler_;
    };

    //////////////////////////////////////////////////////////////////////////
    class _EXPORT_LIBEVENTPP PipedEventWatcher : public EventWatcher {
    public:
        PipedEventWatcher(struct event_base *event_base,
            const Handler& handler);

        void Notify();
    private:
        virtual bool DoInit();
        virtual void DoClose();
        static void HandlerFn(int fd, short which, void *v);

        int pipe_[2];
    };

    //////////////////////////////////////////////////////////////////////////
    class _EXPORT_LIBEVENTPP TimedEventWatcher : public EventWatcher {
    public:
        TimedEventWatcher(struct event_base *event_base,
            const Handler& handler);

		bool AsyncWait(uint64_t timeout_us) { return Watch(timeout_us); }

    private:
        virtual bool DoInit();
        static void HandlerFn(int fd, short which, void *v);
    };

	typedef TimedEventWatcher EventTimer;

    //////////////////////////////////////////////////////////////////////////
#ifdef H_OS_LINUX
    class SignalEventWatcher : public EventWatcher {
    public:
        SignalEventWatcher(int signo, struct event_base *event_base,
            const Handler& handler);

    private:
        virtual bool DoInit();
        static void HandlerFn(int sn, short which, void *v);

        int signo_;
    };
#endif
} // namespace 

#endif // MX_HTTP_LIBEVENT_EVENT_H_


