

#ifndef LIBEVQING_UDP_MESSAGE_H_
#define LIBEVQING_UDP_MESSAGE_H_

#include "evqing/include/inner_pre.h"

namespace evqing
{
    class _EXPORT_EVQING UdpMessage : public osl::MemoryDataStream
    {
    public:
        UdpMessage(SOCKET fd, osl::u32 default_buffer_size = 1472)
            : MemoryDataStream(default_buffer_size), sockfd_(fd)
        {
            init();
        }

        void set_remote_addr(const struct sockaddr& addrForRemoteUdpClient);
        const struct sockaddr* remote_addr() const;
        struct sockaddr* mutable_remote_addr() { return sockaddr_cast(&remote_addr_); }

        SOCKET sockfd() const { return sockfd_; }
    private:
        struct sockaddr_in remote_addr_;
        SOCKET sockfd_;

    private:
        void init()
        {
            memset(&remote_addr_, 0, sizeof(remote_addr_));
        }
    };
    typedef osl::RefPtr<UdpMessage> UdpMessagePtr;

    inline void UdpMessage::set_remote_addr(const struct sockaddr& addrForRemoteUdpClient)
    {
        memcpy(&remote_addr_, &addrForRemoteUdpClient, sizeof addrForRemoteUdpClient);
    }

    inline const struct sockaddr* UdpMessage::remote_addr() const
    {
        return sockaddr_cast(&remote_addr_);
    }

    inline bool SendMessage(SOCKET fd, const struct sockaddr* addr, const char* d, size_t dlen)
    {
        //XXX TODO Do we need a lock here to call sendto ? 
        int sentn = ::sendto(fd, d, dlen, 0, addr, sizeof(*addr));

        if (sentn != (int)dlen) {
            //LOG_ERROR << "sentn=" << sentn << " , dlen=" << dlen;
            return false;
        }

        return true;
    }

    inline bool SendMessage(const UdpMessagePtr& msg)
    {
        return SendMessage(msg->sockfd(), msg->remote_addr(), msg->data(), msg->size());
    }

}
#endif //LIBEVQING_UDP_MESSAGE_H_


