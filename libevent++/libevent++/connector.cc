
#include "evqing/include/inner_pre.h"
// #include "dong/include/socket_utility.h"
// #include "evqing/include/connector.h"
// #include "evqing/include/fd_event.h"
// 
// #include <errno.h>
// 
// namespace evqing {
// 
//     namespace net {
// 
//         const char* Connector::getStateString(Connector::States e)
//         {
//             H_CASE_STRING_BIGIN(e);
//             H_CASE_STRING(kDisconnected);
//             H_CASE_STRING(kConnecting);
//             H_CASE_STRING(kConnected);
//             H_CASE_STRING_END();
//         }
// 
//         const int Connector::kMaxRetryDelayMs = 30 * 1000;
//         const int Connector::kInitRetryDelayMs = 500;
// 
//         Connector::Connector(struct event_base* loop, const dong::net::InetAddress& serverAddr)
//             : loop_(loop),
//             serverAddr_(serverAddr),
//             connect_(false),
//             state_(kDisconnected),
//             retryDelayMs_(kInitRetryDelayMs), 
//             timeout_us_(1000*1000)
//         {
//             LOG_DEBUG << "ctor[" << this << "]";
//         }
// 
//         Connector::~Connector()
//         {
//             LOG_DEBUG << "dtor[" << this << "]";
//             //assert(!channel_);
//         }
// 
//         void Connector::Start()
//         {
//             LOG_TRACE << "Connector::" << __func__;
//             //loop_->assertInLoopThread();
//             assert(state_ == kDisconnected);
//             if (connect_)
//             {
//                 connect();
//             }
//             else
//             {
//                 LOG_DEBUG << "do not connect";
//             }
//         }
// 
// //         void Connector::Stop()
// //         {
// //             connect_ = false;
// //             loop_->queueInLoop(std::tr1::bind(&Connector::stopInLoop, this)); // FIXME: unsafe
// //             // FIXME: cancel timer
// //         }
// // 
// //         void Connector::stopInLoop()
// //         {
// //             loop_->assertInLoopThread();
// //             if (state_ == kConnecting)
// //             {
// //                 setState(kDisconnected);
// //                 int sockfd = removeAndResetChannel();
// //                 retry(sockfd);
// //             }
// //         }
// 
//         bool Connector::connect()
//         {
//             sockfd_ = dong::net::sockets::createNonblockingOrDie();
//             int ret = dong::net::sockets::connect(sockfd_, serverAddr_.getSockAddrInet());
//             const int savedErrno = (ret == 0) ? 0 : osl::SocketUtility::getLastError();
//             if (ret < 0) {
//                 if (EVUTIL_ERR_CONNECT_RETRIABLE(savedErrno))   {
//                     return connecting();
//                 }
//                 return false;
//             }
// 
//             return connecting();
// 
// 
// //             switch (savedErrno)
// //             {
// //             case 0:
// //             case EINPROGRESS:
// //             case EINTR:
// //             case EISCONN:
// // #ifdef H_OS_WINDOWS
// //             case WSAEWOULDBLOCK:
// // #endif
// //                 connecting(sockfd);
// //                 break;
// // 
// //             case EAGAIN:
// //             case EADDRINUSE:
// //             case EADDRNOTAVAIL:
// //             case ECONNREFUSED:
// //             case ENETUNREACH:
// //                 retry(sockfd);
// //                 break;
// // 
// //             case EACCES:
// //             case EPERM:
// //             case EAFNOSUPPORT:
// //             case EALREADY:
// //             case EBADF:
// //             case EFAULT:
// //             case ENOTSOCK:
// //                 LOG_SYSERR << "connect error in Connector::startInLoop " << savedErrno;
// //                 sockets::close(sockfd);
// //                 break;
// // 
// //             default:
// //                 LOG_SYSERR << "Unexpected error in Connector::startInLoop " << savedErrno;
// //                 sockets::close(sockfd);
// //                 // connectErrorCallback_();
// //                 break;
// //             }
//         }
// 
// //         void Connector::restart()
// //         {
// //             loop_->assertInLoopThread();
// //             setState(kDisconnected);
// //             retryDelayMs_ = kInitRetryDelayMs;
// //             connect_ = true;
// //             Start();
// //         }
// 
//         bool Connector::connecting()
//         {
//             setState(kConnecting);
//             fd_event_.reset(new FdEvent(loop_));
//             fd_event_->AsyncWait(sockfd_, FdEvent::kWritable, std::tr1::bind(&Connector::handleEvent, this, std::tr1::placeholders::_1), 1000*1000);
//             return true;
// //             setState(kConnecting);
// //             assert(!channel_);
// //             channel_.reset(new Channel(loop_, sockfd));
// //             channel_->setWriteCallback(
// //                 std::tr1::bind(&Connector::handleWrite, this)); // FIXME: unsafe
// //             channel_->setErrorCallback(
// //                 std::tr1::bind(&Connector::handleError, this)); // FIXME: unsafe
// // 
// //             // channel_->tie(shared_from_this()); is not working,
// //             // as channel_ is not managed by shared_ptr
// //             channel_->enableWriting();
//         }
// 
// //         int Connector::removeAndResetChannel()
// //         {
// //             channel_->disableAll();
// //             channel_->remove();
// //             int sockfd = channel_->fd();
// //             // Can't reset channel_ here, because we are inside Channel::handleEvent
// //             loop_->queueInLoop(std::tr1::bind(&Connector::resetChannel, this)); // FIXME: unsafe
// //             return sockfd;
// //         }
// 
// //         void Connector::resetChannel()
// //         {
// //             channel_.reset();
// //         }
// 
//         void Connector::handleWrite()
//         {
//             LOG_TRACE << "Connector::handleWrite " << getStateString(state_);
// 
//             if (state_ == kConnecting)
//             {
//                 //int sockfd = removeAndResetChannel();
//                 SOCKET sockfd = sockfd_;
//                 int err = dong::net::sockets::getSocketError(sockfd);
//                 if (err)
//                 {
//                     LOG_WARN << "Connector::handleWrite - SO_ERROR = "
//                         << err << " " << dong::strerror_tl(err);
//                     //retry(sockfd);
//                     //TODO
//                 }
//                 else if (dong::net::sockets::isSelfConnect(sockfd))
//                 {
//                     LOG_WARN << "Connector::handleWrite - Self connect";
//                     //retry(sockfd);
//                     //TODO
//                 }
//                 else
//                 {
//                     setState(kConnected);
//                     if (connect_)
//                     {
//                         newConnectionCallback_(sockfd);
//                     }
//                     else
//                     {
//                         dong::net::sockets::close(sockfd);
//                     }
//                 }
//             }
//             else
//             {
//                 // what happened?
//                 assert(state_ == kDisconnected);
//             }
//         }
// 
// //         void Connector::handleError()
// //         {
// //             LOG_ERROR << "Connector::handleError state=" << getStateString(state_);
// //             if (state_ == kConnecting)
// //             {
// //                 int sockfd = removeAndResetChannel();
// //                 int err = sockets::getSocketError(sockfd);
// //                 LOG_TRACE << "SO_ERROR = " << err << " " << strerror_tl(err);
// //                 retry(sockfd);
// //             }
// //         }
// 
//         void Connector::handleEvent(int events)
//         {
//             if (events & FdEvent::kWritable) {
//                 handleWrite();
//             } else {
// 
//             }
//         }
// 
// //         void Connector::retry(int sockfd)
// //         {
// //             sockets::close(sockfd);
// //             setState(kDisconnected);
// //             if (connect_)
// //             {
// //                 LOG_INFO << "Connector::retry - Retry connecting to " << serverAddr_.toIpPort()
// //                     << " in " << retryDelayMs_ << " milliseconds. ";
// //                 loop_->runAfter(retryDelayMs_ / 1000.0,
// //                     std::tr1::bind(&Connector::Start, shared_from_this()));
// //                 retryDelayMs_ = std::min(retryDelayMs_ * 2, kMaxRetryDelayMs);
// //             }
// //             else
// //             {
// //                 LOG_DEBUG << "do not connect";
// //             }
// //         }
// 
//     }
// }
// 
