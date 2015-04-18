/********************************************************************
created:	2010/04/09   10:52
filename: 	E:\cloudcom\src\evqing\include\udp_server.h
file path:	E:\cloudcom\src\evqing\include
author:		weizili

purpose:	UDP Server class. We use this class to receive and send UDP packet data.
And we will start one pairs of threads:
the sender thread focuses on sending data,
the receiver thread focuses on receiving data.
*********************************************************************/
#ifndef LIBEVQING_UDP_SERVER_H_
#define LIBEVQING_UDP_SERVER_H_

#include "evqing/include/inner_pre.h"
#include "udp_message.h"

namespace evqing
{
    class _EXPORT_EVQING UdpServer : public osl::Object
    {
    public:
        typedef std::tr1::function<void(const UdpMessagePtr msg)> MessageHandler;
    public:
        UdpServer();
        ~UdpServer();

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

        void SetMessageHandler(MessageHandler handler);

    public:
        void set_name(const osl::string& n);

    private:
        class Impl;
        std::tr1::shared_ptr<Impl> impl_;
    };

    typedef osl::RefPtr<UdpServer>  UdpServerPtr;


};

#endif


