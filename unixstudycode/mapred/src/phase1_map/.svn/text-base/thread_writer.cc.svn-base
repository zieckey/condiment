#include "thread_writer.h"

#include <fstream>
#include <gflags/gflags.h>

#include "qoslib/include/QThread.h"
#include "qoslib/include/QMyOS.h"
#include "qoslib/include/QLock.h"
#include "qoslib/include/QEvent.h"

#include "constant.h"
#include "common.h"
#include "qlog.h"

DECLARE_int32(dump_buffer_size);

namespace
{//{{{
    //return the time string, the last 2 chars represent second
    std::string GetTimeString()
    {
        std::string::value_type szTime[128] = {};
        struct tm *pTime;
        time_t ctTime;
        time( &ctTime );
        pTime = localtime( &ctTime );

        snprintf( szTime, sizeof(szTime), "%4d%.2d%.2d%.2d%.2d%.2d", 
                    pTime->tm_year + 1900, pTime->tm_mon + 1, pTime->tm_mday,
                    pTime->tm_hour, pTime->tm_min, pTime->tm_sec);
        return std::string(szTime);
    }
}
//}}}

class ThreadWriter::WThread : public osl::Thread
{//{{{
    public:
#ifdef _DEBUG
        WThread(FILE* fp, const std::string& path)
#else
        WThread(FILE* fp)
#endif
            : fp_(fp)
        {
            running_ = false;
#ifdef _DEBUG
            output_path_ = path;
#endif
        }

        ~WThread()
        { 
            TRACE("entering ... ref=%d", getRef());
            Flush();
            fp_ = NULL; 
#ifdef _DEBUG
            qLogTraces(kLogName) << "ThreadWriter::WThread::~WThread() called! data_buf_list_.size()=" << data_buf_list_.size();
            all_data_buf_.toText();
            qLogTraces(kLogName) << "--------------\n" << (char*)all_data_buf_.getCache();
#endif
        }

        //called from main thread
        void Push(osl::MemoryDataStreamPtr& data_buf)
        {        
            if (data_buf->getSize() == 0)
            {
                return;
            }

            {
                H_AUTOLOCK(data_buf_list_lock_);
                data_buf_list_.push_back(data_buf);
                data_buf_list_changed_event_.signal();
            }
        }

        void Flush()
        {
#ifdef _DEBUG
            all_data_buf_.saveToFile(output_path_);
#else
            fflush(fp_);
#endif
        }

        virtual void stop()
        {
            running_ = false;
            data_buf_list_changed_event_.signal();
            osl::Thread::stop();
        }

    private:
        FILE* fp_;

        typedef std::list <osl::MemoryDataStreamPtr> MemoryDataStreamPtrList;
        MemoryDataStreamPtrList data_buf_list_;
        osl::Lock               data_buf_list_lock_;
        osl::Event              data_buf_list_changed_event_;

        bool    running_;

#ifdef _DEBUG
        osl::MemoryDataStream all_data_buf_;
        std::string output_path_;
#endif
    private:

        std::string GetTempOutputFilePath(int64_t file_id)
        {
            char buf[512] = {};
            snprintf(buf, sizeof(buf), "Inc_%ld_%s_%u.tmp", 
                        file_id, GetTimeString().c_str(), 0);	

            return buf;
        }

        virtual void run()
        {
            running_ = true;

            osl::MemoryDataStreamPtr ptr_data_buf;

            while (running_ || !data_buf_list_.empty())
            {
                if (data_buf_list_.empty())
                {
                    data_buf_list_changed_event_.wait();
                }

                while(!data_buf_list_.empty())
                {
                    {
                        H_AUTOLOCK(data_buf_list_lock_);   
                        TRACE("pop a data_buf");
                        ptr_data_buf = data_buf_list_.front();
                        data_buf_list_.pop_front();

#ifdef _DEBUG
                        qLogTraces(kLogName) 
                            << __PRETTY_FUNCTION__ << " Pop data to write, size=" << ptr_data_buf->getSize()
                            << " data_buf_list_ remain count=" << data_buf_list_.size();
#endif

                    }

#ifdef _DEBUG //{{{
                    {
                        static std::string outfile_tmp = GetTempOutputFilePath(0);
                        std::fstream ofp(outfile_tmp.c_str(), std::ios::out|std::ios::binary|std::ios::app);
                        ofp.write((char*)ptr_data_buf->getCache(), ptr_data_buf->getSize());
                        ofp.flush();
                        ofp.close();
                    }
#endif//}}}

#ifdef _DEBUG
                    all_data_buf_.write(ptr_data_buf->getCache(), ptr_data_buf->getSize());
#else
                    int wn = fwrite(ptr_data_buf->getCache(), 1, ptr_data_buf->getSize(), fp_);
                    if (wn < 0)
                    {
                        int myerr = errno;
                        INFO("fwrite error, errno=%d:%s", myerr, strerror(myerr));
                        wn = fwrite(ptr_data_buf->getCache(), 1, ptr_data_buf->getSize(), fp_);
                        if (wn < 0)
                        {
                            myerr = errno;
                            INFO("fwrite error, errno=%d:%s \t exiting... ", myerr, strerror(myerr));
                            exit(-1);
                        }
                    }
                    else if ((long)ptr_data_buf->getSize() > wn)
                    {
                        INFO("fwrite ret=%d, continue to fwrite", wn);
                        int ret = fwrite(ptr_data_buf->data() + wn, 1, ptr_data_buf->size() - wn, fp_);
                        if (ret != (long)ptr_data_buf->size() - wn)
                        {
                            int myerr = errno;
                            INFO("fwrite error, errno=%d:%s \t exiting... ", myerr, strerror(myerr));
                            exit(-1);
                        }
                    }
#endif
                }
            }//end of while

            Flush();
        }
};//}}}

#ifdef _DEBUG
ThreadWriter::ThreadWriter(FILE* fp, const std::string& path) 
#else
ThreadWriter::ThreadWriter(FILE* fp) 
#endif
    : FileWriter(fp) 
    , dump_buffer_max_(FLAGS_dump_buffer_size)
    , output_buf_(new osl::MemoryDataStream(FLAGS_dump_buffer_size))
{
#ifdef _DEBUG
    thread_ = new WThread(fp, path);
#else
    thread_ = new WThread(fp);
#endif
    thread_->ref();
}

ThreadWriter::~ThreadWriter() 
{ 
    TRACE("entering ...");

    Flush();
    thread_->unref();
    thread_ = NULL;

#ifdef _DEBUG
    qLogTraces(kLogName) << "output_buf_->size()=" << output_buf_->getSize();
#endif
    TRACE("leaving ...");
}

bool ThreadWriter::Write(const void* data, size_t len)
{
    TRACE("Entering, data=%s len=%lu", (len == 32 ? std::string((char*)data, len).c_str() : "..."), len);

    if (output_buf_->getSize() + len >= dump_buffer_max_)
    {
#ifdef _DEBUG
        qLogTraces(kLogName) 
            << "Add current buf to thread writer, current data_buf_.size()=" << output_buf_->getSize() 
            << " dump_buffer_max_=" << dump_buffer_max_;
#endif

        thread_->Push(output_buf_);
        TRACE("Push a output to TWriter, data len=%lu", output_buf_->size());
        output_buf_ = (new osl::MemoryDataStream(dump_buffer_max_));
    }

    output_buf_->write(data, len);

    return true;
}

bool ThreadWriter::Flush()
{//{{{
    TRACE("entering ...");

#ifdef _DEBUG
    output_buf_->toText();
    qLogTraces(kLogName) << __func__ << " called: output_buf_->size()=" << output_buf_->getSize() << " contents:\n" << (output_buf_->getSize() > 0 ? (char*)output_buf_->getCache() : "") << "\n----------------";
#endif

    if (output_buf_->getSize() > 0)
    {
        thread_->Push(output_buf_);
        TRACE("Push a output to TWriter, data len=%lu", output_buf_->size());
        output_buf_ = new osl::MemoryDataStream(1);
    }

    //waiting for the thread to write all the data and stop
    while(thread_->isRunning())
    {
        thread_->stop();
        usleep(1000);
    }

    return true;
}//}}}

bool ThreadWriter::Init() 
{ 
    return thread_->start(); 
}


