
#ifndef LIBEVENTPP_CONDITION_H_
#define LIBEVENTPP_CONDITION_H_

#include "inner_pre.h"
#include "libevent_watcher.h"

struct event;
struct event_base;

namespace evpp {
    
    class _EXPORT_LIBEVENTPP Condition : public PipedEventWatcher {
    public:
        Condition(const Handler& handler);
        ~Condition() {}

        bool Init(struct event_base* event_base);
    };
        

} // namespace asmc

#endif // MX_HTTP_LIBEVENT_EVENT_H_


