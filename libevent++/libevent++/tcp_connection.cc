#include "evqing/include/inner_pre.h"
// 
// #include "evqing/include/tcp_connection.h"
// #include "dong/include/socket_utility.h"
// 
// namespace evqing
// {
//     namespace net {
// 
//         TcpConnection::TcpConnection(struct event_base* loop,
//             const std::string& nameArg,
//             SOCKET sockfd,
//             const InetAddress& localAddr,
//             const InetAddress& peerAddr)
//             : loop_(CHECK_NOTNULL(loop)),
//             name_(nameArg),
//             state_(kConnecting),
//             //socket_(new Socket(sockfd)),
//             //channel_(new Channel(loop, sockfd)),
//             localAddr_(localAddr),
//             peerAddr_(peerAddr),
//             highWaterMark_(64 * 1024 * 1024),
//             sockfd_(sockfd)
//         {
// //             channel_->setReadCallback(
// //                 std::tr1::bind(&TcpConnection::handleRead, this, std::tr1::placeholders::_1));
// //             channel_->setWriteCallback(
// //                 std::tr1::bind(&TcpConnection::handleWrite, this));
// //             channel_->setCloseCallback(
// //                 std::tr1::bind(&TcpConnection::handleClose, this));
// //             channel_->setErrorCallback(
// //                 std::tr1::bind(&TcpConnection::handleError, this));
//             LOG_DEBUG << "TcpConnection::ctor[" << name_ << "] at " << this
//                 << " fd=" << sockfd;
//             //socket_->setKeepAlive(true);
//         }
// 
//         TcpConnection::~TcpConnection()
//         {
//             LOG_DEBUG << "TcpConnection::dtor[" << name_ << "] at " << this
//                 << " fd=" << sockfd_;
//         }
// 
//         void TcpConnection::send(const void* data, int len)
//         {
//             send(StringPiece(static_cast<const char*>(data), len));
//         }
// 
//         void TcpConnection::send(const StringPiece& message)
//         {
//             if (state_ == kConnected)
//             {
//                 sendInLoop(message);
// //                 if (loop_->isInLoopThread())
// //                 {
// //                     sendInLoop(message);
// //                 }
// //                 else
// //                 {
// //                     loop_->runInLoop(
// //                         std::tr1::bind(&TcpConnection::sendStringInLoop,
// //                         this,
// //                         message.toString()));
// //                     //std::forward<string>(message)));
// //                 }
//             }
//         }
// 
//         // FIXME efficiency!!!
// //         void TcpConnection::send(Buffer* buf)
// //         {
// //             if (state_ == kConnected)
// //             {
// //                 if (loop_->isInLoopThread())
// //                 {
// //                     sendInLoop(buf->peek(), buf->readableBytes());
// //                     buf->retrieveAll();
// //                 }
// //                 else
// //                 {
// //                     string smessage = buf->retrieveAllAsString();
// //                     loop_->runInLoop(
// //                         std::tr1::bind(&TcpConnection::sendStringInLoop,
// //                         this,
// //                         smessage));
// //                     //std::forward<string>(message)));
// //                 }
// //             }
// //         }
// 
//         void TcpConnection::sendStringInLoop(const std::string message)
//         {
//             sendInLoop(message.data(), message.size());
//         }
// 
//         void TcpConnection::sendInLoop(const StringPiece& message)
//         {
//             sendInLoop(message.data(), message.size());
//         }
// 
//         void TcpConnection::sendInLoop(const void* data, size_t len)
//         {
//             //loop_->assertInLoopThread();
//             ssize_t nwrote = 0;
//             size_t remaining = len;
//             bool faultError = false;
//             if (state_ == kDisconnected)
//             {
//                 LOG_WARN << "disconnected, give up writing";
//                 return;
//             }
//             // if no thing in output queue, try writing directly
//             if (!channel_->isWriting() && outputBuffer_.readableBytes() == 0)
//             {
//                 nwrote = sockets::write(channel_->fd(), data, len);
//                 if (nwrote >= 0)
//                 {
//                     remaining = len - nwrote;
//                     if (remaining == 0 && writeCompleteCallback_)
//                     {
//                         loop_->queueInLoop(std::tr1::bind(writeCompleteCallback_, shared_from_this()));
//                     }
//                 }
//                 else // nwrote < 0
//                 {
//                     nwrote = 0;
//                     if (errno != EWOULDBLOCK)
//                     {
//                         LOG_SYSERR << "TcpConnection::sendInLoop";
//                         if (
// #ifndef H_OS_WINDOWS
//                             errno == EPIPE ||
// #endif
//                             errno == ECONNRESET) // FIXME: any others?
//                         {
//                             faultError = true;
//                         }
//                     }
//                 }
//             }
// 
//             assert(remaining <= len);
//             if (!faultError && remaining > 0)
//             {
//                 size_t oldLen = outputBuffer_.readableBytes();
//                 if (oldLen + remaining >= highWaterMark_
//                     && oldLen < highWaterMark_
//                     && highWaterMarkCallback_)
//                 {
//                     loop_->queueInLoop(std::tr1::bind(highWaterMarkCallback_, shared_from_this(), oldLen + remaining));
//                 }
//                 outputBuffer_.append(static_cast<const char*>(data)+nwrote, remaining);
//                 if (!channel_->isWriting())
//                 {
//                     channel_->enableWriting();
//                 }
//             }
//         }
// 
//         void TcpConnection::shutdown()
//         {
//             // FIXME: use compare and swap
//             if (state_ == kConnected)
//             {
//                 setState(kDisconnecting);
//                 // FIXME: shared_from_this()?
//                 //loop_->runInLoop(std::tr1::bind(&TcpConnection::shutdownInLoop, this));
//                 shutdownInLoop();
//             }
//         }
// 
//         void TcpConnection::shutdownInLoop()
//         {
//             //loop_->assertInLoopThread();
// //             if (!channel_->isWriting())
// //             {
// //                 // we are not writing
// //                 socket_->shutdownWrite();
// //             }
//             dong::net::sockets::shutdownWrite(sockfd_);
//         }
// 
// //         void TcpConnection::setTcpNoDelay(bool on)
// //         {
// //             //socket_->setTcpNoDelay(on);
// //         }
// 
//         void TcpConnection::connectEstablished()
//         {
//             loop_->assertInLoopThread();
//             assert(state_ == kConnecting);
//             setState(kConnected);
//             channel_->tie(shared_from_this());
//             channel_->enableReading();
// 
//             connectionCallback_(shared_from_this());
//         }
// 
//         void TcpConnection::connectDestroyed()
//         {
//             loop_->assertInLoopThread();
//             if (state_ == kConnected)
//             {
//                 setState(kDisconnected);
//                 channel_->disableAll();
// 
//                 connectionCallback_(shared_from_this());
//             }
//             channel_->remove();
//         }
// 
//         void TcpConnection::handleRead(Timestamp receiveTime)
//         {
//             loop_->assertInLoopThread();
//             int savedErrno = 0;
//             ssize_t n = inputBuffer_.readFd(channel_->fd(), &savedErrno);
//             if (n > 0)
//             {
//                 messageCallback_(shared_from_this(), &inputBuffer_, receiveTime);
//             }
//             else if (n == 0)
//             {
//                 handleClose();
//             }
//             else
//             {
//                 errno = savedErrno;
//                 LOG_SYSERR << "TcpConnection::handleRead";
//                 handleError();
//             }
//         }
// 
//         void TcpConnection::handleWrite()
//         {
//             loop_->assertInLoopThread();
//             if (channel_->isWriting())
//             {
//                 ssize_t n = sockets::write(channel_->fd(),
//                     outputBuffer_.peek(),
//                     outputBuffer_.readableBytes());
//                 if (n > 0)
//                 {
//                     outputBuffer_.retrieve(n);
//                     if (outputBuffer_.readableBytes() == 0)
//                     {
//                         channel_->disableWriting();
//                         if (writeCompleteCallback_)
//                         {
//                             loop_->queueInLoop(std::tr1::bind(writeCompleteCallback_, shared_from_this()));
//                         }
//                         if (state_ == kDisconnecting)
//                         {
//                             shutdownInLoop();
//                         }
//                     }
//                 }
//                 else
//                 {
//                     LOG_SYSERR << "TcpConnection::handleWrite";
//                     // if (state_ == kDisconnecting)
//                     // {
//                     //   shutdownInLoop();
//                     // }
//                 }
//             }
//             else
//             {
//                 LOG_TRACE << "Connection fd = " << channel_->fd()
//                     << " is down, no more writing";
//             }
//         }
// 
//         void TcpConnection::handleClose()
//         {
//             loop_->assertInLoopThread();
//             LOG_TRACE << "fd = " << channel_->fd() << " state = " << (int)state_;
//             assert(state_ == kConnected || state_ == kDisconnecting);
//             // we don't close fd, leave it to dtor, so we can find leaks easily.
//             setState(kDisconnected);
//             channel_->disableAll();
// 
//             TcpConnectionPtr guardThis(shared_from_this());
//             connectionCallback_(guardThis);
//             // must be the last line
//             closeCallback_(guardThis);
//         }
// 
//         void TcpConnection::handleError()
//         {
//             int err = sockets::getSocketError(channel_->fd());
//             LOG_ERROR << "TcpConnection::handleError [" << name_
//                 << "] - SO_ERROR = " << err << " " << osl::strerror_tl(err);
//         }
// 
//     }
// }