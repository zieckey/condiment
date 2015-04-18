/********************************************************************
*	Created:	2015/04/17 21:28
*	File name: 	HTTP_context.h
*	Author:		weizili
*	Purpose:
*
*	Copyright 2010-2015, All Rights Reserved.
********************************************************************/
#ifndef _EMBEDDED_HTTP_CONTEXT_H_
#define _EMBEDDED_HTTP_CONTEXT_H_

#include "osl/include/exp.h"
#include "osl/include/noncopyable.h"

#include "evqing/include/inner_pre.h"

struct event_base;
struct evhttp;
struct evhttp_request;

namespace evqing
{
    struct _EXPORT_EVQING HTTPContext
    {
        std::string uri; // The URI without parameters
        struct evhttp_request* req;

        const char* remote_ip() const;  // Get the remote ip address of the http client
        const char* original_uri() const; // Get the original URI with parameters
    };

    typedef std::tr1::function<
        void(const std::string& response_data)
    > HTTPSendResponseCallback;

    typedef std::map<std::string, std::string> HTTPParameterMap;
    typedef std::tr1::function<
        void(const HTTPContext& ctx,
        const HTTPParameterMap& params,
        HTTPSendResponseCallback respcb) > HTTPRequestCallback;

    typedef std::map<std::string, HTTPRequestCallback> HTTPRequestCallbackMap;
}

#endif

