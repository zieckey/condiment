#include "session.h"
#include "client.h"

AtomicInt32 Session::totalSession_;

void Session::onConnection(const TcpConnectionPtr& conn)
{
  if (conn->connected())
  {
    LOG_INFO << "id=" << id_ << " connected to " << conn->peerAddress().toIpPort();
    conn->setTcpNoDelay(true);
    sendMessage(conn);
    owner_->onConnect();
  }
  else
  {
    LOG_INFO << "id=" << id_ << " " << conn->peerAddress().toIpPort() << " disconnetted";
    owner_->onDisconnect(conn);
  }
}


void Session::sendMessage(const TcpConnectionPtr& conn)
{
  if (messagesRead_ >= loopCount_) {
    LOG_INFO << "id=" << id_ << " have been sent " << messagesRead_ << " messages , all done";
    //conn->shutdown();
    tcp_client_.disconnect();
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

