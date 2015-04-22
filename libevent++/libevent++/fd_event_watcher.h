
#ifndef LIBEVENTPP_FD_EVENT_WATCHER_H_
#define LIBEVENTPP_FD_EVENT_WATCHER_H_

#include "libevent_watcher.h"
#include "timestamp.h"

#include <boost/function.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace evpp {
    class _EXPORT_LIBEVENTPP FdEventWatcher : public EventWatcher {
    public:
        enum EventType {
            kReadable = 0x02,
            kWritable = 0x04,
        };
        typedef boost::function<void()> EventCallback;
        typedef boost::function<void(Timestamp)> ReadEventCallback;

    public:
        FdEventWatcher(struct event_base *evbase, int fd);

        void set_read_callback(const ReadEventCallback& cb) {
            read_cb_ = cb;
        }
        void set_write_callback(const EventCallback& cb) {
            write_cb_ = cb;
        }

    private:
        virtual bool DoInit();
        virtual void DoClose();
        void HandlerFn(int fd, short which);

        static void HandlerFn(int fd, short which, void *v);

    private:
        ReadEventCallback read_cb_;
        EventCallback write_cb_;
        EventCallback close_cb_;
        EventCallback error_cb_;

        int fd_;
    };

} // namespace 

#endif // MX_HTTP_LIBEVENT_EVENT_H_


