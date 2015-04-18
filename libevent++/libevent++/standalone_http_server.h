/********************************************************************
 *	Created:	2015/05/16 22:26
 *	File name: 	standalone_http_server.h
 *	Author:		weizili
 *	Purpose:	
 *
 *	Copyright 2010-2015, All Rights Reserved.
 ********************************************************************/
#ifndef EVQING_STANDLONE_HTTP_SERVER_H_
#define EVQING_STANDLONE_HTTP_SERVER_H_


#include "osl/include/exp.h"
#include "osl/include/thread.h"

#include "evqing/include/embedded_http_server.h"

struct event_base;
struct event;

namespace evqing
{
    class _EXPORT_EVQING StandaloneHTTPServer
    {
    public:
        StandaloneHTTPServer();

        ~StandaloneHTTPServer();

		//! \brief Create a thread to run the http service event loop.
		//! \param[in] - int http_listening_port
		//! \param[in] - bool wait_until_http_service_started
		//!			if true : this call will be blocked until the http service has started.
		//!			if false : this call will return immediately and the http service may hasn't started.
		//! \return - bool
		bool Start(int http_listening_port) { return Start(http_listening_port, false); }
		bool Start(int http_listening_port, bool wait_until_http_service_started);

        //! \brief Stop the http server. Thread safe.
        //! \param[in] - bool wait_thread_exit : true this function will blocked until the running thread exits
        //! \return - bool - 
        bool Stop(bool wait_thread_exit = false);

	public:
		bool IsRunning() const;
		bool IsStopped() const;

        struct event_base* event_base() const;

    public:
        bool RegisterEvent(const std::string& uri,
            HTTPRequestCallback callback);

        bool RegisterDefaultEvent(HTTPRequestCallback callback);

    private:
        class Impl;
        std::tr1::shared_ptr<Impl> impl_;
    };
}

#endif
