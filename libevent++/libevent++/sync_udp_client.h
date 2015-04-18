#ifndef LIBEVQING_SYNC_UDP_CLIENT_H_
#define LIBEVQING_SYNC_UDP_CLIENT_H_

#include "evqing/include/inner_pre.h"
#include "evqing/include/udp_message.h"

namespace evqing 
{ 	
	namespace sync
	{
		class _EXPORT_EVQING UdpClient
		{
		public:
			//! Constructors
			//! \param sockfd, 
			//!     If it equals to INVALID_SOCKET, you need to call <code>connect(...)</code> method to connect to UDP server
			//!     else you can use this instance to receive and send data directly.
			UdpClient();
			~UdpClient();

			//! \brief This will call the native socket API 
			//!		'int connect(int sockfd, const struct sockaddr *serv_addr, socklen_t addrlen)'
			//!		So it is the same 
			//! \return false if can not the connect address specified by szHostname and szService.
			bool Connect(const char* szHostname, const char* szService);
			bool Connect(const char* szHostname, int port);
			bool Connect(const osl::string& strHostName, const osl::string& strService);
			bool Connect(const struct sockaddr& addr);
			bool Connect(const struct sockaddr_in& addr);

			//! Disconnect the pipe from remote side.
			//! \remark It is usually called by application user when want to disconnect the pipe.
			void Close();

            bool Send(const osl::string& msg);
            bool Send(const char* msg, size_t len);

			//! brief : Do a udp request and wait for remote udp server send response data
			//! param[in] - const osl::string & udp_package_data
			//! return - osl::string the response data
			osl::string DoRequest(const osl::string& udp_package_data, uint32_t timeout_ms);

			static osl::string DoRequest(const osl::string& remote_ip, int port, const osl::string& udp_package_data, uint32_t timeout_ms);

            static bool Send(const osl::string& msg, const struct sockaddr_in& addr);
            static bool Send(const char* msg, size_t len, const struct sockaddr_in& addr);
            static bool Send(const UdpMessagePtr& msg);
        public:
            SOCKET sockfd() const {
                return sockfd_;
            }
		private:
			friend class UdpClientReceiverThread;
			bool Connect();

			enum EventType {
				kTimeout,
				kRecv,
				kSend,
				kError,
			};
			EventType Poll(uint32_t timeout_ms);
			osl::string RecvMessage();
		private:
			SOCKET sockfd_;
			struct sockaddr remote_addr_;
		};
	}
}

#endif // LIBEVQING_SYNC_UDP_CLIENT_H_
