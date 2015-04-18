#include "evqing/include/inner_pre.h"

#include "osl/include/logger.h"
#include "evqing/include/sync_udp_client.h"
#include "evqing/include/udp_message.h"

namespace evqing
{
	namespace sync
	{
		UdpClient::UdpClient()
		{
			sockfd_ = INVALID_SOCKET;
			memset(&remote_addr_, 0, sizeof(remote_addr_));
		}

		//----------------------------------------------
		UdpClient::~UdpClient(void)
		{
			Close();
		}

		//----------------------------------------------
		bool UdpClient::Connect(const char* szHostname, const char* szService)
		{
			osl::NetUtility::set_address(szHostname, szService, "udp", (struct sockaddr_in*)&remote_addr_);
			return Connect();
		}

		bool UdpClient::Connect(const struct sockaddr_in& addr)
		{
			memcpy(&remote_addr_, &addr, sizeof(remote_addr_));
			return Connect();
		}

		bool UdpClient::Connect(const char* szHostname, int port)
		{
			std::string service = osl::StringUtil::valueOf(port);
			return Connect(szHostname, service.data());
		}

		bool UdpClient::Connect(const struct sockaddr& addr)
		{
			memcpy(&remote_addr_, &addr, sizeof(remote_addr_));
			return Connect();
		}

		bool UdpClient::Connect()
		{
			sockfd_ = osl::NetUtility::udp_client();
			
			// reuse address.
			if (!osl::NetUtility::setSocketReuse(sockfd_, true))
			{
				Close();
				LOG_ERROR << "Failed to call setSocketReuse, errno=" << osl::NetUtility::getLastSocketError();
				return false;
			}

// 			if (!osl::NetUtility::setnonblocking(sockfd_))
// 			{
// 				Close();
// 				LOG_ERROR << "Failed to call setnonblocking, errno=" << osl::NetUtility::getLastSocketError();
// 				return false;
// 			}


			socklen_t addrlen = sizeof(remote_addr_);
			int ret = ::connect(sockfd_, (struct sockaddr*)&remote_addr_, addrlen);

			if (ret != 0)
			{
				Close();
				struct sockaddr_in *paddr = (struct sockaddr_in*)&remote_addr_;
				LOG_ERROR << "Failed to connect to remote IP="
					<< inet_ntoa(paddr->sin_addr)
					<< ", port=" << ntohs(paddr->sin_port)
					<< ", errno=" << osl::NetUtility::getLastSocketError();

				return false;
			}
			else
			{
			}

			return true;
		}


		//----------------------------------------------
		void UdpClient::Close()
		{
			H_CLOSE_SOCKET(sockfd_);
		}


		osl::string UdpClient::DoRequest(const osl::string& data, uint32_t timeout_ms)
		{
			if (!Send(data)) {
				LOG_ERROR << "sent failed, errno=" << osl::SocketUtility::getLastError() << " , dlen=" << data.size();
				return "";
			}

			osl::SocketUtility::setTimeout(sockfd_, timeout_ms);

			osl::u32 nBufSize = 1472; // The UDP max payload size
			UdpMessagePtr msg = H_NEW UdpMessage(sockfd_, nBufSize);
			socklen_t m_nAddrLen = sizeof(remote_addr_);
			int readn = ::recvfrom(sockfd_, msg->cache(), nBufSize, 0, msg->mutable_remote_addr(), &m_nAddrLen);
			int err = osl::SocketUtility::getLastError();
			if (readn == 0)
			{
				//TODO ERROR process
				LOG_ERROR << "errno=" << err << " " << strerror(err) << " recvfrom return 0";
			}
			else if (readn > 0)
			{
				msg->seekp(readn);
				return osl::string(msg->data(), msg->size());
			}
			else
			{
				//TODO

				LOG_ERROR << "errno=" << err << " " << strerror(err) << " recvfrom return -1";
			}

			return "";
		}

// 		UdpClient::EventType UdpClient::Poll(uint32_t timeout_ms)
// 		{
// 			fd_set rs;
// 			fd_set ws;
// 			fd_set es;
// 
// 			FD_ZERO(&rs);
// 			FD_ZERO(&ws);
// 			FD_ZERO(&es);
// 
// 			
// 			FD_SET(sockfd_, &rs);
// 			FD_SET(sockfd_, &ws);
// 			FD_SET(sockfd_, &es);
// 
// 			ws = rs;
// 			es = rs;
// 
// 			struct timeval tv;
// 			tv.tv_sec = timeout_ms/1000;
// 			tv.tv_usec = (timeout_ms%1000)*1000;
// 
// 			int selret = select(sockfd_ + 1, &rs, &ws, &es, &tv);
// 
// 			// err occurs
// 			if (-1 == selret)
// 			{
// 				int myerrno = osl::SocketUtility::getLastError();
// 				LOG_INFO << "err when selecting in recv:"
// 					<< ", errno=" << myerrno << ", " << strerror(errno);
// 				return kError;
// 			}
// 
// 			// time out
// 			if (0 == selret)
// 			{
// 				return kTimeout;
// 			}
// 
// 			osl::SocketUtility::setLastError(0);
// 			if (FD_ISSET(sockfd_, &rs))
// 			{
// 				return kRecv;
// 			}
// 
// 			if (FD_ISSET(sockfd_, &ws))
// 			{
// 				return kSend;
// 			}
// 			return kError;
// 		}
// 
// 		osl::string UdpClient::DoRequest(const osl::string& data, uint32_t timeout_ms)
// 		{
// 			int sentn = ::sendto(sockfd_,
// 				(const char*)data.data(),
// 				data.size(), 0, &remote_addr_, sizeof(remote_addr_));
// 
// 			if (sentn != (int)data.size()) {
// 				LOG_ERROR << "sentn=" << sentn << " , dlen=" << data.size();
// 				return "";
// 			}
// 
// 			int event_type = Poll(timeout_ms);
// 			switch (event_type)
// 			{
// 			case kRecv:
// 				return RecvMessage();
// 			default:
//				break;
// 			}
// 
// 			return "";
// 		}
// 
// 		osl::string UdpClient::RecvMessage()
// 		{
// 			osl::u32 nBufSize = 1472; // The UDP max payload size
// 			UdpMessagePtr msg = H_NEW UdpMessage(sockfd_, nBufSize);
// 			socklen_t m_nAddrLen = sizeof(remote_addr_);
// 			int readn = ::recvfrom(sockfd_, msg->cache(), nBufSize, 0, msg->mutable_remote_addr(), &m_nAddrLen);
// 			int err = osl::SocketUtility::getLastError();
// 			if (readn == 0)
// 			{
// 				//TODO ERROR process
// 				LOG_ERROR << "errno=" << err << " " << strerror(err) << " recvfrom return 0";
// 			}
// 			else if (readn > 0)
// 			{
// 				msg->seekp(readn);
// 				return osl::string(msg->data(), msg->size());
// 			}
// 			else
// 			{
// 				//TODO
// 
// 				LOG_ERROR << "errno=" << err << " " << strerror(err) << " recvfrom return -1";
// 			}
// 
// 			return "";
// 		}

		osl::string UdpClient::DoRequest(const osl::string& remote_ip, int port, const osl::string& udp_package_data, uint32_t timeout_ms)
		{
			UdpClient c;
			if (!c.Connect(remote_ip.data(), port)) {
				return "";
			}

			return c.DoRequest(udp_package_data, timeout_ms);
		}

        bool UdpClient::Send(const char* msg, size_t len)
        {
            int sentn = ::sendto(sockfd(),
                msg,
                len, 0, &remote_addr_, sizeof(remote_addr_));
            return sentn > 0;
        }

        bool UdpClient::Send(const osl::string& msg)
        {
            return Send(msg.data(), msg.size());
        }

        bool UdpClient::Send(const osl::string& msg, const struct sockaddr_in& addr)
        {
            return UdpClient::Send(msg.data(), msg.size(), addr);
        }


        bool UdpClient::Send(const char* msg, size_t len, const struct sockaddr_in& addr)
        {
            UdpClient c;
            if (!c.Connect(addr)) {
                return "";
            }

            return c.Send(msg, len);
        }

        bool UdpClient::Send(const UdpMessagePtr& msg)
        {
            return UdpClient::Send(msg->data(), msg->size(), *(const struct sockaddr_in*)(msg->remote_addr()));
        }

    }
}


