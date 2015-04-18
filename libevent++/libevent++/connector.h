// 
// 
// #ifndef EVQING_NET_CONNECTOR_H
// #define EVQING_NET_CONNECTOR_H
// 
// #include "evqing/include/inner_pre.h"
// // #include "dong/include/inet_address.h"
// // #include "dong/include/exp.h"
// // #include "dong/include/inet_address.h"
// 
// struct event_base;
// namespace evqing
// {
//     class FdEvent;
// 	namespace net
// 	{
// 		class _EXPORT_EVQING Connector : osl::noncopyable,
// 			public std::tr1::enable_shared_from_this<Connector>
// 		{
// 		public:
// 			typedef std::tr1::function<void(SOCKET sockfd)> NewConnectionCallback;
// 
//             Connector(struct event_base* loop, const dong::net::InetAddress& serverAddr);
// 			~Connector();
// 
// 			void SetNewConnectionCallback(const NewConnectionCallback& cb)
// 			{
// 				newConnectionCallback_ = cb;
// 			}
// 
//             void Start();  // called by the event_base thread
// 			void Stop();  // called by the event_base thread
// 
//             const dong::net::InetAddress& serverAddress() const { return serverAddr_; }
// 
// 		private:
// 			static const int kMaxRetryDelayMs;
// 			static const int kInitRetryDelayMs;
// 			enum States { kDisconnected, kConnecting, kConnected };
// 			const char* getStateString(States e);
// 			void setState(States s) { state_ = s; }
// 			
// 			//void stopInLoop();
// 			bool connect();
//             bool connecting();
//             void handleEvent(int events);
// 			void handleWrite();
// 			//void handleError();
// 			//void retry(int sockfd);
// 			//int removeAndResetChannel();
// 			//void resetChannel();
// 
// 			event_base* loop_;
//             dong::net::InetAddress serverAddr_;
// 			bool connect_; // atomic
// 			States state_;  // FIXME: use atomic variable
// 			//std::tr1::scoped_ptr<Channel> channel_;
// 			NewConnectionCallback newConnectionCallback_;
// 			int retryDelayMs_;
//             int timeout_us_;
//             SOCKET sockfd_;
//             std::tr1::shared_ptr<FdEvent> fd_event_;
// 		};
// 
// 	}
// }
// 
// #endif  // DONG_NET_CONNECTOR_H
