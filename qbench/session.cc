#include "session.h"
#include "client.h"

void Session::onConnection(const TcpConnectionPtr& conn)
{
  if (conn->connected())
  {
    LOG_INFO << "connected to " << conn->peerAddress().toIpPort();
    conn->setTcpNoDelay(true);
    sendMessage(conn);
    owner_->onConnect();
  }
  else
  {
    LOG_INFO << conn->peerAddress().toIpPort() << " not connetted";
    owner_->onDisconnect(conn);
  }
}


void Session::sendMessage(const TcpConnectionPtr& conn)
{
  if (messagesRead_ >= loopCount_) {
    LOG_INFO << "have been sent " << messagesRead_ << " messages , all done";
    conn->shutdown();
    return;
  }
  int64_t now = Timestamp::now().microSecondsSinceEpoch();
  if (lastSendTime_us_ + sendMsgInterval_us_ <= now) {
    conn->send(owner_->message());
    lastSendTime_us_ = now; 
  } else {
      double after = (lastSendTime_us_ + sendMsgInterval_us_ - now)/1000000;
    conn->getLoop()->runAfter(after, 
                boost::bind(&Session::sendMessage, this, conn));
  }
}

