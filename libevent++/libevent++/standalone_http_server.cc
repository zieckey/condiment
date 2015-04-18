#include "evqing/include/inner_pre.h"

#include "evqing/include/standalone_http_server.h"
#include "evqing/include/libevent_condition.h"
#include "evqing/include/libevent_headers.h"

#include <errno.h>


namespace evqing
{
    class StandaloneHTTPServer::Impl
    {
    public:
        Impl();

        ~Impl();

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

        struct event_base* event_base() const {
            return base_;
        }

    public:
        bool RegisterEvent(const std::string& uri,
            HTTPRequestCallback callback);

        bool RegisterDefaultEvent(HTTPRequestCallback callback);

    protected:
        void RunInThread();

        void StopEventLoop();

    private:
        struct event_base* base_;
        std::tr1::shared_ptr<HTTPService>   http_;
        osl::ThreadRunnerPtr thread_;

        std::tr1::shared_ptr<evqing::Condition> stopping_event_;
    };


    StandaloneHTTPServer::Impl::Impl()
    {
        base_ = event_base_new();
        http_.reset(new HTTPService(base_));
    }

    StandaloneHTTPServer::Impl::~Impl()
    {
        stopping_event_.reset();
        http_.reset();
        if (base_) {
            event_base_free(base_);
            base_ = NULL;
        }
    }

    bool StandaloneHTTPServer::Impl::Start(int port, bool wait_for_http_service_started)
	{
		if (!http_->Listen(port)) {
			return false;
		}

		thread_ = new osl::ThreadRunner(std::tr1::bind(&StandaloneHTTPServer::Impl::RunInThread, this));
		bool r = thread_->start();
		if (!r)
		{
			return false;
		}

		if (wait_for_http_service_started)
		{
			while (!IsRunning()) {
				osl::Process::msleep(1);
			}
		}

		return r;
	}


    void StandaloneHTTPServer::Impl::RunInThread()
    {
        stopping_event_.reset(new evqing::Condition(std::tr1::bind(&StandaloneHTTPServer::Impl::StopEventLoop, this)));
        stopping_event_->Init(base_);
        stopping_event_->Watch(uint64_t(0));

        int r = event_base_loop(base_, 0);
        if (r != 0)
        {
            std::cerr << "loop error!\n";
        }
        else {
            //loop exiting ...
        }
    }

    bool StandaloneHTTPServer::Impl::Stop(bool wait_thread_exit /*= false*/)
    {
        stopping_event_->Notify();

        if (wait_thread_exit)
        {
            while (!thread_->isStopped()) {
                osl::Process::msleep(1);
            }
        }

        return true;
    }

    void StandaloneHTTPServer::Impl::StopEventLoop()
    {
        http_->Stop();

        timeval tv;
        tv.tv_sec  = 0;
        tv.tv_usec = 500 * 1000; //500ms
        event_base_loopexit(base_, &tv);
    }

    bool StandaloneHTTPServer::Impl::IsRunning() const
	{
		return thread_->isRunning();
	}

    bool StandaloneHTTPServer::Impl::IsStopped() const
	{
		return thread_->isStopped();
	}

    bool StandaloneHTTPServer::Impl::RegisterEvent(const std::string& uri, HTTPRequestCallback callback)
    {
        return http_->RegisterEvent(uri, callback);
    }

    bool StandaloneHTTPServer::Impl::RegisterDefaultEvent(HTTPRequestCallback callback)
    {
        return http_->RegisterDefaultEvent(callback);
    }


    StandaloneHTTPServer::StandaloneHTTPServer()
    {
        impl_.reset(new Impl());
    }

    StandaloneHTTPServer::~StandaloneHTTPServer()
    {
    }

    bool StandaloneHTTPServer::Start(int port, bool wait_for_http_service_started)
    {
        return impl_->Start(port, wait_for_http_service_started);
    }


    bool StandaloneHTTPServer::Stop(bool wait_thread_exit /*= false*/)
    {
        return impl_->Stop(wait_thread_exit);
    }
    
    bool StandaloneHTTPServer::IsRunning() const
    {
        return impl_->IsRunning();
    }

    bool StandaloneHTTPServer::IsStopped() const
    {
        return impl_->IsStopped();
    }

    struct event_base* StandaloneHTTPServer::event_base() const
    {
        return impl_->event_base();
    }

    bool StandaloneHTTPServer::RegisterEvent(const std::string& uri, HTTPRequestCallback callback)
    {
        return impl_->RegisterEvent(uri, callback);
    }

    bool StandaloneHTTPServer::RegisterDefaultEvent(HTTPRequestCallback callback)
    {
        return impl_->RegisterDefaultEvent(callback);
    }

}


