
#ifndef LIBEVENTPP_FD_EVENT_WATCHER_H_
#define LIBEVENTPP_FD_EVENT_WATCHER_H_

#include "libevent_watcher.h"
#include "timestamp.h"

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

struct event;
struct event_base;

namespace evpp {
    class _EXPORT_LIBEVENTPP FdChannel {
    public:
        enum EventType {
            kNone     = 0x00,
            kReadable = 0x02,
            kWritable = 0x04,
        };
        typedef boost::function<void()> EventCallback;
        typedef boost::function<void(Timestamp)> ReadEventCallback;

    public:
        FdChannel(struct event_base *evbase, int fd,
            bool watch_read_event, bool watch_write_event);

        bool Start();
        void Close();

        void set_read_callback(const ReadEventCallback& cb) {
            read_fn_ = cb;
        }

        void set_write_callback(const EventCallback& cb) {
            write_fn_ = cb;
        }

        int fd() const { return fd_; }
    private:
        void HandlerFn(int fd, short which);
        static void HandlerFn(int fd, short which, void *v);

    private:
        ReadEventCallback read_fn_;
        EventCallback write_fn_;

        struct event*      event_;
        struct event_base* evbase_;

        int events_;
        int fd_;
    };

} // namespace 

#endif // MX_HTTP_LIBEVENT_EVENT_H_


