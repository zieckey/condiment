#include "evqing/include/inner_pre.h"

#include "osl/include/logger.h"
#include "osl/include/socket_utility.h"
#include "osl/include/thread.h"

#include "evqing/include/udp_server.h"
#include "evqing/include/udp_message.h"

namespace evqing
{
    class UdpServer::Impl
    {
    public:
        Impl();
        ~Impl();

        //! Start the server.
        //! \remark Start the service and listening in the given port
        //!		This call will start several receiving thread at every net interface
        //! \return false if failed to start server.
        bool Start(int port);

        //! Start the server.
        //! \remark Start the service at the given ip and listening in the given port
        //!		This call will start only one receiving thread at the given ip
        //! \return false if failed to start server.
        bool Start(const osl::string& ip, int port);

        //! Stop the server
        void Stop();

        void SetMessageHandler(MessageHandler handler) {
            message_handler_ = handler;
        }

    public:
        void set_name(const osl::string& n);

    private:
        struct RecvThread
        {
            SOCKET				 sockfd;
            Impl*			     udp_server;
            osl::string		     ip;
            int					 port;
            osl::ThreadRunnerPtr thread;
        };
        typedef std::tr1::shared_ptr<RecvThread> RecvThreadPtr;

        typedef std::vector<RecvThreadPtr> RecvThreadVector;
        RecvThreadVector recv_threads_;
        MessageHandler   message_handler_;

        //For unit test
    private:
        std::string      name_;
    private:
        void RecvingLoop(RecvThread* th);
    };

    UdpServer::Impl::Impl()
    {
    }

    UdpServer::Impl::~Impl()
    {
    }

    bool UdpServer::Impl::Start(int port)
    {
        osl::SocketUtility::HostInfoList hosts = osl::SocketUtility::getAllNetInterface();
        osl::SocketUtility::HostInfoList::iterator it(hosts.begin());
        osl::SocketUtility::HostInfoList::iterator ite(hosts.end());
        for (; it != ite; ++it)
        {
            if (!Start(it->strIP, port))
            {
                return false;
            }
        }
        return true;
    }

    bool UdpServer::Impl::Start(const osl::string& ip, int port)
    {
        if (!message_handler_) {
            LOG_ERROR << "MessageHandler DO NOT set!";
            return false;
        }
        osl::string servicename = osl::StringUtil::valueOf(port);
        
        RecvThreadPtr th(new RecvThread);
        th->ip = ip;
        th->port = port;
        th->sockfd = osl::SocketUtility::udp_server(ip.data(), servicename.data());
        th->udp_server = this;
        th->thread = new osl::ThreadRunner(std::tr1::bind(&UdpServer::Impl::RecvingLoop, this, th.get()));
        osl::SocketUtility::setTimeout(th->sockfd, 500);
        if (!th->thread->start())
        {
            LOG_ERROR << "start recv thread at (" << th->ip << ":" << port << ") failed";
            return false;
        }
        LOG_TRACE << "start udp server at " << ip << ":" << port;
        recv_threads_.push_back(th);
        return true;
    }

    void UdpServer::Impl::Stop()
    {
        RecvThreadVector::iterator it(recv_threads_.begin());
        RecvThreadVector::iterator ite(recv_threads_.end());
        for (; it != ite; it++)
        {
            (*it)->thread->stop();
            //it->thread->terminate();//TODO FIXME. the thread maybe haven't stopped
        }
    }

    void UdpServer::Impl::RecvingLoop(RecvThread* th)
    {
        while (th->thread->isRunning())
        {
            osl::u32 nBufSize = 1472; // The UDP max payload size
            UdpMessagePtr pRecvNetMsg = H_NEW UdpMessage(th->sockfd, nBufSize);
            socklen_t m_nAddrLen = sizeof(struct sockaddr);
            int readn = ::recvfrom(th->sockfd, (char*)pRecvNetMsg->cache(), nBufSize, 0, pRecvNetMsg->mutable_remote_addr(), &m_nAddrLen);
            LOG_TRACE << "fd=" << th->sockfd << " port=" << th->port
                << " recved len=" << readn << " from " << osl::SocketUtility::toIpPort(*pRecvNetMsg->remote_addr());
            if (readn == 0)
            {
                //TODO ERROR process
                LOG_ERROR << strerror(osl::SocketUtility::getLastError()) << " recvfrom return 0";
            }
            else if (readn > 0)
            {
                pRecvNetMsg->seekp(readn);
                th->udp_server->message_handler_(pRecvNetMsg);
            }
            else
            {
                
                    int myerrno = osl::SocketUtility::getLastError();
#ifdef H_OS_WINDOWS
                    if (myerrno == WSAEWOULDBLOCK)
#else
                    if (myerrno == EAGAIN)
#endif
                    {
                        //TODO ERROR process
                        osl::Process::usleep(1);
                    }

                    LOG_ERROR << "errno=" << myerrno << " " << strerror(myerrno);
                
            }
        }

        LOG_INFO << "fd=" << th->sockfd << " port=" << th->port << " UDP server(" << name_ << ") existed.";
        H_CLOSE_SOCKET(th->sockfd);
    }

    void UdpServer::Impl::set_name(const osl::string& n)
    {
        name_ = n;
    }



    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////


    void UdpServer::set_name(const osl::string& n)
    {
        impl_->set_name(n);
    }
    void UdpServer::SetMessageHandler(MessageHandler handler)
    {
        impl_->SetMessageHandler(handler);
    }

    bool UdpServer::Start(int port)
    {
        return impl_->Start(port);
    }

    bool UdpServer::Start(const osl::string& ip, int port)
    {
        return impl_->Start(ip, port);
    }

    void UdpServer::Stop()
    {
        return impl_->Stop();
    }

    UdpServer::UdpServer()
    {
        impl_.reset(new UdpServer::Impl);
    }

    UdpServer::~UdpServer()
    {
        impl_->Stop();
        impl_.reset();
    }

}
