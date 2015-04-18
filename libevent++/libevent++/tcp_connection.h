// #ifndef EVENGQING_TCP_CONNECTION_H_
// #define  EVENGQING_TCP_CONNECTION_H_
// 
// #include "evqing/include/inner_pre.h"
// //#include "dong/include/callbacks.h"
// 
// struct event_base;
// struct event;
// 
// namespace evqing {
//     namespace net
//     {
// 
//         class TcpConnection;
//         typedef std::tr1::shared_ptr<TcpConnection> TcpConnectionPtr;
//         typedef std::tr1::function<void()> TimerCallback;
//         typedef std::tr1::function<void(const TcpConnectionPtr&)> ConnectionCallback;
//         typedef std::tr1::function<void(const TcpConnectionPtr&)> CloseCallback;
//         typedef std::tr1::function<void(const TcpConnectionPtr&)> WriteCompleteCallback;
//         typedef std::tr1::function<void(const TcpConnectionPtr&, size_t)> HighWaterMarkCallback;
// 
//         // the data has been read to (buf, len)
//         typedef std::tr1::function<void(const TcpConnectionPtr&,
//             Buffer*,
//             osl::Timestamp)> MessageCallback;
// 
//         ///
//         /// TCP connection, for both client and server usage.
//         ///
//         /// This is an interface class, so don't expose too much details.
//         class _EXPORT_EVQING TcpConnection : public osl::noncopyable,
//             public std::tr1::enable_shared_from_this<TcpConnection>
//         {
//         public:
//             /// Constructs a TcpConnection with a connected sockfd
//             ///
//             /// User should not create this object.
//             TcpConnection(struct event_base* loop,
//                 const std::string& name,
//                 SOCKET sockfd,
//                 const InetAddress& localAddr,
//                 const InetAddress& peerAddr);
//             ~TcpConnection();
// 
//             //struct event_base* getLoop() const { return loop_; }
//             const std::string& name() const { return name_; }
//             const InetAddress& localAddress() { return localAddr_; }
//             const InetAddress& peerAddress() { return peerAddr_; }
//             bool connected() const { return state_ == kConnected; }
// 
//             // void send(string&& message); // C++11
//             void send(const void* message, int len);
//             void send(const StringPiece& message)
//             // void send(Buffer&& message); // C++11
//             void send(Buffer* message);  // this one will swap data
//             void shutdown(); // NOT thread safe, no simultaneous calling
//             void setTcpNoDelay(bool on);
// 
//             void setContext(const osl::any& context)
//             { context_ = context; }
// 
//             const osl::any& getContext() const
//             { return context_; }
// 
//             osl::any* getMutableContext()
//             { return &context_; }
// 
//             void setConnectionCallback(const dong::net::ConnectionCallback& cb)
//             { connectionCallback_ = cb; }
// 
//             void setMessageCallback(const MessageCallback& cb)
//             { messageCallback_ = cb; }
// 
//             void setWriteCompleteCallback(const WriteCompleteCallback& cb)
//             { writeCompleteCallback_ = cb; }
// 
//             void setHighWaterMarkCallback(const HighWaterMarkCallback& cb, size_t highWaterMark)
//             { highWaterMarkCallback_ = cb; highWaterMark_ = highWaterMark; }
// 
//             /// Advanced interface
//             Buffer* inputBuffer()
//             { return &inputBuffer_; }
// 
//             Buffer* outputBuffer()
//             { return &outputBuffer_; }
// 
//             /// Internal use only.
//             void setCloseCallback(const CloseCallback& cb)
//             { closeCallback_ = cb; }
// 
//             // called when TcpServer accepts a new connection
//             void connectEstablished();   // should be called only once
//             // called when TcpServer has removed me from its map
//             void connectDestroyed();  // should be called only once
// 
//         private:
//             enum StateE { kDisconnected, kConnecting, kConnected, kDisconnecting };
//             void handleRead(Timestamp receiveTime);
//             void handleWrite();
//             void handleClose();
//             void handleError();
//             //void sendInLoop(string&& message);
//             void sendStringInLoop(const string message);
//             void sendInLoop(const StringPiece& message);
//             void sendInLoop(const void* message, size_t len);
//             void shutdownInLoop();
//             void setState(StateE s) { state_ = s; }
// 
//             struct event_base* loop_;
//             std::string name_;
//             StateE state_;  // FIXME: use atomic variable
//             // we don't expose those classes to client.
//             // std::tr1::scoped_ptr<Socket> socket_;
//             // std::tr1::scoped_ptr<Channel> channel_;
//             InetAddress localAddr_;
//             InetAddress peerAddr_;
//             ConnectionCallback connectionCallback_;
//             MessageCallback messageCallback_;
//             WriteCompleteCallback writeCompleteCallback_;
//             HighWaterMarkCallback highWaterMarkCallback_;
//             CloseCallback closeCallback_;
//             size_t highWaterMark_;
//             Buffer inputBuffer_;
//             Buffer outputBuffer_; // FIXME: use list<Buffer> as output buffer.
//             osl::any context_;
//             SOCKET sockfd_;
//             // FIXME: creationTime_, lastReceiveTime_
//             //        bytesReceived_, bytesSent_
//         };
// 
//         typedef std::tr1::shared_ptr<TcpConnection> TcpConnectionPtr;
// 
//     }
// }
// #endif
// 
