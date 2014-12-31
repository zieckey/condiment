#ifndef BETCH_SESSION_H
#define BETCH_SESSION_H

#include <muduo/net/TcpClient.h>
#include <muduo/base/Atomic.h>
#include <muduo/base/Logging.h>
#include <muduo/base/Thread.h>
#include <muduo/net/EventLoop.h>
#include <muduo/net/EventLoopThreadPool.h>
#include <muduo/net/InetAddress.h>

#include <boost/bind.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include <utility>

#include <mcheck.h>
#include <stdio.h>
#include <unistd.h>

using namespace muduo;
using namespace muduo::net;

class Client;

class Session : boost::noncopyable
{
 public:
  Session(EventLoop* loop,
          const InetAddress& serverAddr,
          const string& name,
          double sendMsgInterval,
          int loopCount,
          Client* owner)
    : tcp_client_(loop, serverAddr, name),
      owner_(owner),
      lastSendTime_us_(Timestamp::now().microSecondsSinceEpoch()),
      sendMsgInterval_us_(int64_t(sendMsgInterval * 1000 * 1000)),
      loopCount_(loopCount),
      bytesRead_(0),
      bytesWritten_(0),
      messagesRead_(0)
  {
    tcp_client_.setConnectionCallback(
        boost::bind(&Session::onConnection, this, _1));
    tcp_client_.setMessageCallback(
        boost::bind(&Session::onMessage, this, _1, _2, _3));

    tcp_client_.enableRetry();

    id_ = totalSession_.incrementAndGet();
  }

  void start()
  {
    tcp_client_.connect();
  }

  void stop()
  {
    tcp_client_.disconnect();
  }

  int64_t bytesRead() const
  {
     return bytesRead_;
  }

  int64_t messagesRead() const
  {
     return messagesRead_;
  }

 private:

  void sendMessage(const TcpConnectionPtr& conn);

  void onConnection(const TcpConnectionPtr& conn);

  void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp)
  {
    LOG_DEBUG << "id=" << id_ 
        << " recv from " << conn->peerAddress().toIpPort() 
        << " localIpPort=" << conn->localAddress().toIpPort()
        << " len=" << buf->readableBytes();
    ++messagesRead_;
    bytesRead_ += buf->readableBytes();
    bytesWritten_ += buf->readableBytes();
    sendMessage(conn);
    buf->retrieveAll();
  }

  TcpClient tcp_client_;
  Client* owner_;
  int64_t lastSendTime_us_;
  int64_t  sendMsgInterval_us_; //micro second
  int     loopCount_;
  int64_t bytesRead_;
  int64_t bytesWritten_;
  int64_t messagesRead_;

  int64_t id_;

  static AtomicInt32 totalSession_;
};


#endif

