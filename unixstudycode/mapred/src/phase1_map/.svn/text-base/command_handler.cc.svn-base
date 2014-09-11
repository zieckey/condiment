#include "command_handler.h"

#include <fstream>

#include "qoslib/include/QOSLibAllExp.h"
#include "qoslib/include/QFileUtil.h"
#include "qoslib/include/QTimer.h"
#include "qoslib/include/QThread.h"
#include "qoslib/include/QEvent.h"
#include "qoslib/include/QRingBuffer.h"
#include "qoslib/include/QMD5.h"

#include "constant.h"
#include "common.h"
#include "qlog.h"

#include <gflags/gflags.h>

#include "buffer_writer.h"
#include "thread_writer.h"

//DEFINE_string(file_writer_type, "BufferWriter", "The actually output writer type, options : BufferWriter, ThreadWriter");

DECLARE_bool(enable_thread_worker);
DECLARE_int32(input_buffer_size);

class CommandHandler::ThreadHandler : public osl::Thread
{
    typedef std::vector < osl::MemoryDataStreamPtr > MemoryDataStreamPtrList;
    //typedef std::list < osl::MemoryDataStreamPtr > MemoryDataStreamPtrList;
public:
    ThreadHandler(CommandHandler* command_handler)
        : processing_(false), command_handler_(command_handler)
    {
    }

    bool Work(osl::MemoryDataStreamPtr& buf)
    {
        bool ok = true;
#ifdef USING_RINGBUFFER
        assert(buf && buf->getReadableSize() > 0);
        TRACE("Push back,buf:ref=%d ReadableSize=%u tellg=%u size=%u", buf->getRef(), buf->getReadableSize(), buf->tellg(), buf->size());
        buf->ref();
        ok = ring_buffer_.push_back(static_cast<void*>(buf.getPointer()));
#else
        {
            H_AUTOLOCK(data_buf_list_lock_);
            data_buf_list_.push_back(buf);
        }
#endif
        if (ok)
        {
            if (processing_ == false)
            {
                event_.signal();
            }
        }
        return ok;
    }

    void stop()
    {
#ifdef USING_RINGBUFFER 
        while (ring_buffer_.size() > 0)
#else
        while (data_buf_list_.size() > 0 || data_buf_cache_list_.size() > 0)
#endif
        {
            if (processing_ == false)
            {
                event_.signal();
            }
            TRACE("Waiting process finished!");
            usleep(10);
        }

        osl::Thread::stop();

        while (!isStopped())
        {
            event_.signal();
            usleep(10);
        }
    }

private:
    virtual void run()
    {
        osl::MemoryDataStreamPtr last_buf;
        osl::MemoryDataStreamPtr current_buf;
        while (isRunning())
        {
#ifdef USING_RINGBUFFER
            if (ring_buffer_.size() == 0)
            {
                processing_ = false;
                event_.wait();
            }

            processing_ = true;
            while (ring_buffer_.size() > 0)
            {
                last_buf = current_buf;
                current_buf = static_cast<osl::MemoryDataStream*>(ring_buffer_.pop_front());
                current_buf->unref();//because we called ref() before we push_back the data_stream to ring_buffer_
                assert(current_buf);
                Work(last_buf, current_buf);
            }
#else
            if (data_buf_list_.empty())
            {
                processing_ = false;
                event_.wait();
            }

            if (data_buf_list_.empty())
            {
                continue;
            }

            {
                H_AUTOLOCK(data_buf_list_lock_);
                processing_ = true;
                TRACE("list size=%lu", data_buf_list_.size());
                data_buf_list_.swap(data_buf_cache_list_);
            }

            MemoryDataStreamPtrList::iterator it(data_buf_cache_list_.begin());
            MemoryDataStreamPtrList::iterator ite(data_buf_cache_list_.end());
            for (; it != ite; ++it)
            {
                last_buf = current_buf;
                current_buf = *it;
                Work(last_buf, current_buf);
            }
            data_buf_cache_list_.clear();
#endif
        }
    }

    void Work(osl::MemoryDataStreamPtr& last_buf, osl::MemoryDataStreamPtr& current_buf)
    {
        //--------------------------------------------------------
        //step 1 : check the last time we unfinished data
        assert(current_buf);
        if (last_buf)
        {
            const size_t last_buf_readable_size = last_buf->getReadableSize();
            if (last_buf_readable_size > 0)
            {
                const size_t current_buf_readable_size = current_buf->getReadableSize();
                const char* line_begin = (char*)current_buf->getCurReadBuffer();
                const char* line_end = (char*)memchr(line_begin, '\n', current_buf_readable_size);
                if (!line_end)
                {
                    if (int(last_buf_readable_size + current_buf_readable_size) > FLAGS_input_buffer_size/2)
                    {
                        TRACE("need expand buffer,last_buf_readable_size=%lu current_buf_readable_size=%lu FLAGS_input_buffer_size=%d", last_buf_readable_size, current_buf_readable_size, FLAGS_input_buffer_size);
                        FLAGS_input_buffer_size *= 2;
                        current_buf->reserve(FLAGS_input_buffer_size);
                    }

                    //combination to one string
                    memmove((char*)current_buf->getCache() + last_buf_readable_size, line_begin, current_buf_readable_size);
                    memcpy(current_buf->getCache(), last_buf->getCurReadBuffer(), last_buf_readable_size);
                    current_buf->setSize(last_buf_readable_size + current_buf_readable_size);
                    current_buf->seekg(0 - (int)current_buf->tellg());
                    assert(current_buf->tellg() == 0 && current_buf->size() == last_buf_readable_size + current_buf_readable_size);
                    //all prepared for the next time
                    return;
                }

                const size_t current_line_size = line_end - line_begin;
                current_buf->seekg(current_line_size + 1);
                const size_t line_size = last_buf_readable_size + current_line_size;
                std::string s;
                s.resize(line_size);
                memcpy(&s[0], last_buf->getCurReadBuffer(), last_buf_readable_size);
                memcpy(&s[0] + last_buf_readable_size, line_begin, current_line_size);
                osl::Slice line = osl::Slice(s);
                TRACE("combination a new line='%s'", s.c_str());
                command_handler_->Work(line);
            }
        }

        //--------------------------------------------------------
        //step 2 : 
        const char* line_begin = NULL;
        const char* line_end = NULL;
        size_t current_line_size = 0;
        osl::Slice line;
        while (current_buf->getReadableSize() > 0)
        {
            line_begin = (char*)current_buf->getCurReadBuffer();
            line_end = (char*)memchr(line_begin, '\n', current_buf->getReadableSize());
            if (!line_end)
            {
                TRACE("Cannot find '\\n' in cur_buf, cur_buf.size=%lu cur_buf.readable_size=%lu", current_buf->size(), current_buf->getReadableSize());
                break;
            }
            current_line_size = line_end - line_begin;
            line = osl::Slice(line_begin, line_end - line_begin);
            command_handler_->Work(line);
            current_buf->seekg(current_line_size + 1);
        }
    }

private:
#ifdef USING_RINGBUFFER
    osl::RingBuffer ring_buffer_;
#else
    MemoryDataStreamPtrList data_buf_list_;
    osl::Lock               data_buf_list_lock_;
    MemoryDataStreamPtrList data_buf_cache_list_;
#endif
    osl::Event      event_;
    bool            processing_;

    CommandHandler* command_handler_;
};

//------------------------------------------------------------
//------------------------------------------------------------
//------------------------------------------------------------
CommandHandler::CommandHandler()
    : writer_(NULL),
      thread_handler_(NULL)
{
}

CommandHandler::~CommandHandler()
{
    if (writer_)
    {
        delete writer_;
        writer_ = NULL;
    }

    if (thread_handler_)
    {
        thread_handler_->stop();
        thread_handler_->unref();
        thread_handler_ = NULL;
    }
}

bool CommandHandler::Init(FILE* fp)
{
    if (FLAGS_enable_thread_worker)
    {
        thread_handler_ = new ThreadHandler(this);
        thread_handler_->ref();
        if (!thread_handler_->start())
        {
            fprintf(stderr, "Cannot start command handler thread.\n");
            return false;
        }

        while(!thread_handler_->isRunning())
        {
            usleep(1000);
        }
    }

    writer_ = new BufferWriter(fp);
    return writer_->Init();

    /*
    if (FLAGS_file_writer_type == "BufferWriter")
    {
        writer_ = new BufferWriter(fp);
        return writer_->Init();
    }
    else if (FLAGS_file_writer_type == "ThreadWriter")
    {
        writer_ = new ThreadWriter(fp);
        return writer_->Init();
    }
    else
    {
        fprintf(stderr, "Not supported writer! %s\n", FLAGS_file_writer_type.c_str());
        return false;
    }
    */

}

bool CommandHandler::Flush()
{
    if (FLAGS_enable_thread_worker)
    {
        thread_handler_->stop();
    }

    LastSerialize();
    writer_->Flush();

    return true;
}

bool CommandHandler::WorkBuffer(osl::MemoryDataStreamPtr& buf)
{
    return thread_handler_->Work(buf);
}

