/********************************************************************
*	Created:	2015/04/15 19:28
*	File name: 	embedded_http_server.h
*	Author:		weizili
*	Purpose:
*
*	Copyright 2010-2015, All Rights Reserved.
********************************************************************/
#ifndef _EMBEDDED_HTTP_SERVER_H_
#define _EMBEDDED_HTTP_SERVER_H_

#include "osl/include/exp.h"
#include "osl/include/noncopyable.h"

#include "evqing/include/inner_pre.h"
#include "http_context.h"

#ifdef H_OS_WINDOWS
#include <functional>
#else
#include <tr1/functional>
#endif

#include <map>
#include <string>

namespace evqing
{
    class _EXPORT_EVQING HTTPService {
    public:
        static const std::string kPostData;

    public:
        HTTPService(struct event_base* base);
        ~HTTPService();

        bool Listen(int port);

        void Stop();

        //! \brief URI=/uri?q=value&k=v
        //!     @see unit test <code>http_ParseURI_test</code>
        //! \param[in] - const std::string & uri
        //! \param[in] - RequestCallback callback
        //! \return - bool
        bool RegisterEvent(const std::string& uri, HTTPRequestCallback callback);

        bool RegisterDefaultEvent(HTTPRequestCallback callback);


    public:
        //! \brief Parse URI from the original url
        //!     "/index.php?q=word" ==> uri = "/index.php"
        //! \param[in] - const char * req_uri
        //! \param[out] - std::string & uri
        //! \return std::string - the uri of the URL
        static std::string ParseURI(const char *req_uri);

    private:
        class Impl;
        std::tr1::shared_ptr<Impl> impl_;
    };
}

#endif //#ifndef _MX_H_HTTP_SERVICE_H_

