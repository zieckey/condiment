#ifndef BENCH_CLIENT_H_
#define BENCH_CLIENT_H_

#include "session.h"

class Client : boost::noncopyable
{
 public:
  Client(EventLoop* loop,
         const InetAddress& serverAddr,
         int blockSize,
         int sessionCount,
         int timeout,
         int threadCount,
         int loopCount,
         double sendMsgInterval)
    : loop_(loop),
      threadPool_(loop),
      sessionCount_(sessionCount),
      timeout_(timeout)
  {
    //loop->runAfter(timeout, boost::bind(&Client::handleTimeout, this));
    if (threadCount > 1)
    {
      threadPool_.setThreadNum(threadCount);
    }
    threadPool_.start();

    for (int i = 0; i < blockSize; ++i)
    {
      message_.push_back(static_cast<char>(i % 128));
    }

    for (int i = 0; i < sessionCount; ++i)
    {
      char buf[32];
      snprintf(buf, sizeof buf, "C%05d", i);
      Session* session = new Session(threadPool_.getNextLoop(), serverAddr, buf, sendMsgInterval, loopCount, this);
      session->start();
      sessions_.push_back(session);
    }
  }

  const string& message() const
  {
    return message_;
  }

  void onConnect()
  {
    if (numConnected_.incrementAndGet() == sessionCount_)
    {
      LOG_WARN << "all connected";
    }
  }

  void onDisconnect(const TcpConnectionPtr& conn)
  {
    if (numConnected_.decrementAndGet() == 0)
    {
      LOG_WARN << "all disconnected";

      int64_t totalBytesRead = 0;
      int64_t totalMessagesRead = 0;
      for (boost::ptr_vector<Session>::iterator it = sessions_.begin();
          it != sessions_.end(); ++it)
      {
        totalBytesRead += it->bytesRead();
        totalMessagesRead += it->messagesRead();
      }
      LOG_WARN << totalBytesRead << " total bytes read";
      LOG_WARN << totalMessagesRead << " total messages read";
      LOG_WARN << static_cast<double>(totalBytesRead) / static_cast<double>(totalMessagesRead)
               << " average message size";
      LOG_WARN << static_cast<double>(totalBytesRead) / (timeout_ * 1024 * 1024)
               << " MiB/s throughput";
      conn->getLoop()->queueInLoop(boost::bind(&Client::quit, this));
    }
  }

 private:

  void quit()
  {
    loop_->queueInLoop(boost::bind(&EventLoop::quit, loop_));
  }

  void handleTimeout()
  {
    LOG_WARN << "stop";
    std::for_each(sessions_.begin(), sessions_.end(),
                  boost::mem_fn(&Session::stop));
  }

  EventLoop* loop_;
  EventLoopThreadPool threadPool_;
  int sessionCount_;
  int timeout_;
  boost::ptr_vector<Session> sessions_;
  string message_;
  AtomicInt32 numConnected_;
};

#endif
