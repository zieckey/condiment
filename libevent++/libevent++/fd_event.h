#ifndef EVENTQING_FD_EVENT_H_
#define EVENTQING_FD_EVENT_H_

#include "inner_pre.h"

struct event_base;
struct event;
namespace evqing {

    class _EXPORT_LIBEVENTPP FdEvent {
        static const int kTimedOut = 4;
    public:
        enum Type {
            kReadable = 1,
            kWritable = 2,
        };
        typedef std::tr1::function<void(int /*Type*/)> Functor;

        FdEvent(struct event_base* loop);
        ~FdEvent();

        //! brief : 
        //! param[in] - int fd
        //! param[in] - int events
        //! param[in] - const Functor & handler
        //! param[in] - uint32_t timeout_us - the maximum amount of time to wait for the event, 
        //!             or 0 to wait forever
        //! return - void
        void AsyncWait(int fd, int events, const Functor& handler, uint32_t timeout_us = 0);
        void Cancel();

    protected:
        void Start(int fd, int events, uint32_t timeout_us);
        static void Notify(int fd, short what, void *arg);

        struct event_base* loop_;
        struct event *ev_;
        Functor handler_;
        int8_t flags_;
        bool active_;
    };
}

#endif

