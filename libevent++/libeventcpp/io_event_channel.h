
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
    class _EXPORT_LIBEVENTPP IOEventChannel {
    public:
        enum EventType {
            kNone     = 0x00,
            kReadable = 0x02,
            kWritable = 0x04,
        };
        typedef boost::function<void()> EventCallback;
        typedef boost::function<void(Timestamp)> ReadEventCallback;

    public:
        IOEventChannel(struct event_base *evbase, int fd, bool r, bool w);

        bool Start();
        void Close();

        void set_read_callback(const ReadEventCallback& cb) {
            read_cb_ = cb;
        }
        void set_write_callback(const EventCallback& cb) {
            write_cb_ = cb;
        }

        int fd() const { return fd_; }
    private:
        void HandlerFn(int fd, short which);
        static void HandlerFn(int fd, short which, void *v);

    private:
        ReadEventCallback read_cb_;
        EventCallback write_cb_;
        EventCallback close_cb_;
        EventCallback error_cb_;

        struct event*      event_;
        struct event_base* evbase_;

        int events_;
        int fd_;
    };

} // namespace 

#endif // MX_HTTP_LIBEVENT_EVENT_H_


