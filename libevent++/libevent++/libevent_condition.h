
#ifndef LIBEVENTPP_CONDITION_H_
#define LIBEVENTPP_CONDITION_H_

#include "inner_pre.h"
#include "libevent_watcher.h"

struct event;
struct event_base;

namespace evqing {

#ifndef _DERIVED_FROM_PIPED_EVENT_WATCHER

    class _EXPORT_LIBEVENTPP Condition {
    public:
        typedef std::tr1::function<void()> Handler;

        Condition(const Handler& handler);
        ~Condition();

        bool Init(struct event_base* event_base);

        bool Watch(const struct timeval *timeout = NULL);
		bool Watch(uint64_t timeout_us /*= 0*/);

        void Cancel();
        void Notify();

    private:
        void Close();

    private:
        static void HandleEvent(int, short, void*);

        struct event* event_;
        Handler handler_;
        int pipe_[2];
    };

    // for the sake of compatible
    class _EXPORT_LIBEVENTPP Event : public Condition {
    public:
        Event(const Condition::Handler& handler);
    };


#else
    
    class _EXPORT_LIBEVENTPP Condition : public PipedEventWatcher {
    public:
        Condition(const Handler& handler) : PipedEventWatcher(NULL, handler)
        {
        }
        ~Condition() {}

        bool Init(struct event_base* event_base) {
            set_evbase(event_base);
            return PipedEventWatcher::Init();
        }
    };
        
#endif

} // namespace asmc

#endif // MX_HTTP_LIBEVENT_EVENT_H_


