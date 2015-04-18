#include "inner_pre.h"
#include "libevent_condition.h"

#include "libevent_headers.h"

namespace evqing
{

// #ifndef _DERIVED_FROM_PIPED_EVENT_WATCHER
// Condition::Condition(const Handler& handler)
//     : handler_(handler)
// {
//     pipe_[0] = pipe_[1] = -1;
//     event_ = (struct event*)malloc(sizeof(struct event));
//     memset(event_, 0, sizeof(struct event));
// }
// 
// Condition::~Condition() {
//     Close();
//     if (event_)
//     {
//         event_del(event_);
//         free(event_);
//         event_ = NULL;
//     }
//     
// }
// 
// bool Condition::Init(struct event_base* event_base) {
//     assert(pipe_[0] == -1);
//     if (evutil_socketpair(AF_UNIX, SOCK_STREAM, 0, pipe_) < 0) {
//         goto failed;
//     }
//     if (evutil_make_socket_nonblocking(pipe_[0]) < 0 ||
//         evutil_make_socket_nonblocking(pipe_[1]) < 0) {
//         goto failed;
//     }
// 
//     event_set(event_, pipe_[1], EV_READ | EV_PERSIST,
//         Condition::HandleEvent, this);
// 
//     event_base_set(event_base, event_);
//     return true;
// 
// failed:
//     Close();
//     return false;
// }
// 
// namespace detail
// {
// void close(int fd)
// {
// #ifdef H_OS_WINDOWS
//     ::closesocket(fd);
// #else
//     ::close(fd);
// #endif
// }
// }
// 
// void Condition::Close() {
//     if (pipe_[0] != -1) {
//         detail::close(pipe_[0]);
//         detail::close(pipe_[1]);
//         pipe_[0] = pipe_[1] = -1;
//     }
// }
// 
// bool Condition::Watch(const struct timeval *timeout) {
//     assert(pipe_[0] > 0);
//     if (event_add(event_, timeout) != 0) {
//         return false;
//     }
//     return true;
// }
// 
// bool Condition::Watch(uint64_t timeout_us /*= 0*/)
// {
//     if (timeout_us == 0)
//     {
//         return Watch((const struct timeval *)NULL);
//     }
// 
//     struct timeval tv;
//     tv.tv_sec  = (long)timeout_us / 1000000;
//     tv.tv_usec = (long)timeout_us % 1000000;
// 
//     return Watch(&tv);
// }
// 
// 
// void Condition::Cancel() {
//     assert(pipe_[0] != -1);
//     event_del(event_);
// }
// 
// void Condition::HandleEvent(int fd, short which, void* v) {
//     Condition *e = (Condition*)v;
//     char buf[128];
//     int n = 0;
//     if ((n = recv(e->pipe_[1], buf, sizeof(buf), 0)) > 0) {
//         try {
//             e->handler_();
//         } catch (...) {}
//     }
// }
// 
// void Condition::Notify() {
//     assert(pipe_[0] != -1);
//     char buf[1] = {};
//     if (send(pipe_[0], buf, sizeof(buf), 0) < 0) {
//         return;
//     }
// }
// 
// Event::Event(const Handler& handler) : Condition(handler) {
// }
// #else

Condition::Condition(const Handler& handler) : PipedEventWatcher(NULL, handler)
{
}

bool Condition::Init(struct event_base* event_base)
{
    set_evbase(event_base);
    return PipedEventWatcher::Init();
}

// #endif

}

