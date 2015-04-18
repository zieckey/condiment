#include "evqing/include/inner_pre.h"

#include "osl/include/http_data_parser.h"

#include "evqing/include/embedded_http_server.h"
#include "evqing/include/libevent_condition.h"
#include "evqing/include/libevent_headers.h"



#include <vector>



namespace evqing
{
    const std::string HTTPService::kPostData = "postdata";

    class HTTPService::Impl {
    public:
        Impl(struct event_base* base);
        ~Impl();

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
        void HandleRequest(struct evhttp_request *req);

        void DefaultHandleRequest(HTTPContext ctx, HTTPParameterMap params, struct evhttp_request * req);

        void ParseParameters(struct evhttp_request * req, std::string& uri, HTTPParameterMap &params);

    private:
        static void GenericCallback(struct evhttp_request *req, void *arg);

    private:
        void SendReplyInLoop(struct evhttp_request *req, const std::string& response);

        //thread-safe, it will be called from other thread possibly
        void SendReplyT(struct evhttp_request *req, const std::string& response);

        void HandleReply();

    private:
        struct evhttp*      evhttp_;
        struct event_base*  event_base_;

        HTTPRequestCallbackMap  callbacks_;
        HTTPRequestCallback     default_callback_;

    private:
        struct PendingReply
        {
            PendingReply(struct evhttp_request* r, const std::string& m)
            : req(r), reply(m) {}

            struct evhttp_request*  req;
            std::string             reply;
        };

        typedef std::tr1::shared_ptr<PendingReply> PendingReplyPtr;

        typedef std::list<PendingReplyPtr> PendingReplyPtrList;
        PendingReplyPtrList         pending_reply_list_;
        osl::Lock                   pending_reply_list_lock_;
        std::tr1::shared_ptr<evqing::Condition> pending_reply_list_watcher_;
    };


    HTTPService::Impl::Impl(struct event_base* base /*= NULL*/)
        : evhttp_(NULL), event_base_(base)
    {
        if (!event_base_) {
            return;
        }

        evhttp_ = evhttp_new(event_base_);
        if (!evhttp_) {
            return;
        }
    }

    HTTPService::Impl::~Impl()
    {
        Stop();
    }

    bool HTTPService::Impl::Listen(int port)
    {
        if (!evhttp_ || !event_base_) {
            return false;
        }

        if (evhttp_bind_socket(evhttp_, "0.0.0.0", port) != 0) {
            return false;
        }

        pending_reply_list_watcher_.reset(new evqing::Condition(std::tr1::bind(&HTTPService::Impl::HandleReply, this)));
        pending_reply_list_watcher_->Init(event_base_);
        pending_reply_list_watcher_->Watch((uint64_t)0);

        evhttp_set_gencb(evhttp_, &HTTPService::Impl::GenericCallback, this);

        return true;
    }

    void HTTPService::Impl::Stop()
    {
        if (evhttp_)
        {
            evhttp_free(evhttp_);
            evhttp_ = NULL;
        }
        event_base_ = NULL;
        callbacks_.clear();
        default_callback_ = HTTPRequestCallback();
    }

    bool HTTPService::Impl::RegisterEvent(const std::string& uri, HTTPRequestCallback callback)
    {
        callbacks_[uri] = callback;
        return true;
    }

    bool HTTPService::Impl::RegisterDefaultEvent(HTTPRequestCallback callback)
    {
        default_callback_ = callback;
        return true;
    }

    void HTTPService::Impl::GenericCallback(struct evhttp_request *req, void *arg)
    {
        HTTPService::Impl* hsrv = (HTTPService::Impl*)arg;
        hsrv->HandleRequest(req);
    }

    std::string HTTPService::Impl::ParseURI(const char *req_uri)
    {
        std::string uri;
        if (req_uri == NULL)
            return uri;

        const char* pstart = req_uri;

        if (*pstart != '/')
            return uri;

        const char* pend = strchr(pstart, '?');
        if (pend != NULL)
        {
            if (pend > pstart)
            {
                uri = std::string(pstart, pend - pstart);
            }
        }
        else
        {
            pend = req_uri + strlen(req_uri);
            uri = std::string(pstart, pend - pstart);
        }

        return uri;
    }


    void HTTPService::Impl::ParseParameters(struct evhttp_request * req, std::string& uri, HTTPParameterMap &params)
    {
        osl::URIParser::parse(req->uri, strlen(req->uri), uri, params);

        if (req->type == EVHTTP_REQ_POST)
        {
#if _EVENT_NUMERIC_VERSION >= 0x02001500
            struct evbuffer* evbuf = evhttp_request_get_input_buffer(req);
            size_t buffer_size = evbuffer_get_length(evbuf);
            if (buffer_size > 0){
                std::string body = std::string((char*)evbuffer_pullup(evbuf, -1), buffer_size);
                params[kPostData] = body;
            }
#else
            if (req->input_buffer->off > 0) {
                params[kPostData] = std::string((char*)req->input_buffer->buffer, req->input_buffer->off);
            }
#endif
        }
    }

    void HTTPService::Impl::HandleRequest(struct evhttp_request *req)
    {
        HTTPContext ctx;
        ctx.req = req;

        HTTPParameterMap params;
        ParseParameters(req, ctx.uri, params);

        HTTPRequestCallbackMap::iterator it = callbacks_.find(ctx.uri);
        if (it == callbacks_.end()) {
            DefaultHandleRequest(ctx, params, req);
            return;
        }

        it->second(ctx, params, std::tr1::bind(&HTTPService::Impl::SendReplyT, this, req, std::tr1::placeholders::_1));
    }

    void HTTPService::Impl::DefaultHandleRequest(HTTPContext ctx, HTTPParameterMap params, struct evhttp_request * req)
    {
        if (default_callback_) {
            default_callback_(ctx, params, std::tr1::bind(&HTTPService::Impl::SendReplyT, this, req, std::tr1::placeholders::_1));
        }
        else {
            evhttp_send_reply(req, HTTP_BADREQUEST, "Bad Request", NULL);
        }
    }

    void HTTPService::Impl::SendReplyInLoop(struct evhttp_request *req, const std::string& response)
    {
        if (response.empty())
        {
            evhttp_send_reply(req, HTTP_NOTFOUND, "Not Found", NULL);
            return;
        }

        struct evbuffer *buffer = evbuffer_new();
        evbuffer_add(buffer, response.c_str(), response.size());
        evhttp_send_reply(req, HTTP_OK, "OK", buffer);
        evbuffer_free(buffer);
    }

    void HTTPService::Impl::SendReplyT(struct evhttp_request *req, const std::string& response)
    {
        H_AUTOLOCK(pending_reply_list_lock_);
        PendingReplyPtr fr(new PendingReply(req, response));
        pending_reply_list_.push_back(fr);
        pending_reply_list_watcher_->Notify();
    }

    void HTTPService::Impl::HandleReply()
    {
        PendingReplyPtrList reply_list;

        {
            H_AUTOLOCK(pending_reply_list_lock_);
            reply_list.swap(pending_reply_list_);
        }

        PendingReplyPtrList::iterator it(reply_list.begin());
        PendingReplyPtrList::iterator ite(reply_list.end());
        for (; it != ite; ++it)
        {
            PendingReplyPtr& pt = *it;
            SendReplyInLoop(pt->req, pt->reply);
        }

        reply_list.clear();
    }


    HTTPService::HTTPService(struct event_base* base /*= NULL*/)
    {
        impl_.reset(new Impl(base));
    }

    HTTPService::~HTTPService()
    {
        impl_->Stop();
    }

    bool HTTPService::Listen(int port)
    {
        return impl_->Listen(port);
    }

    void HTTPService::Stop()
    {
        impl_->Stop();
    }

    bool HTTPService::RegisterEvent(const std::string& uri, HTTPRequestCallback callback)
    {
        return impl_->RegisterEvent(uri, callback);
    }

    bool HTTPService::RegisterDefaultEvent(HTTPRequestCallback callback)
    {
       
        return impl_->RegisterDefaultEvent(callback);
    }

    std::string HTTPService::ParseURI(const char *req_uri)
    {
        return Impl::ParseURI(req_uri);
    }
}





