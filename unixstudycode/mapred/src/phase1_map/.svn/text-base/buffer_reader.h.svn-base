#ifndef _FILE_BUFFER_READER_H_
#define _FILE_BUFFER_READER_H_

#include <gflags/gflags.h>

#include "file_reader.h"

#include "qlog.h"
#include "constant.h"
#include "common.h"

DECLARE_int32(input_buffer_size);

DECLARE_bool(enable_thread_worker);

class BufferReader //: public FileReader
{
public:
    BufferReader(FILE* fp)
        //: FileReader(fp)
        : fp_(fp), data_buf_(NULL)
    {
        if (!FLAGS_enable_thread_worker)
        {
            FLAGS_input_buffer_size = FLAGS_input_buffer_size >> 1;
            data_buf_ = CreateNewDataBuffer();        
        }
    }

    ~BufferReader()
    {
    }

    /*virtual*/ bool Init()
    {
        if (0 != feof(fp_))
        {
            //qLogInfos(kLogName) << "file end!";
            return false;
        }

        if (FLAGS_enable_thread_worker)
        {
            return true;
        }
        else
        {
            int readn = fread(data_buf_->getCache(), 1, FLAGS_input_buffer_size, fp_);
            if (readn <= 0)
            {
                return false;
            }

            data_buf_->seekp(readn);

            return true;
        }
    }
        
#ifdef _DEBUG
    #define PrintDebugString() \
    {\
        std::string s((char*)data_buf_->getCurReadBuffer(), 64);\
        data_buf_->toText();\
        qLogTraces(kLogName) \
            << "\n\tbuf_size=" << FLAGS_input_buffer_size\
            << "\n\tread_pos=" << data_buf_->tellg()\
            << "\n\twrite_pos=" << data_buf_->tellp()\
            << "\n\tcontent---------------------\n" \
            << "\n-------------------------------\n"\
            << "next string being:" << s\
            << "\n-------------------------------\n";\
    }
#else
    #define PrintDebugString() {}    
#endif

    /*virtual*/ bool GetLine(osl::Slice& line);

    osl::MemoryDataStreamPtr Read();
private:
    FILE* fp_;
    osl::MemoryDataStream* data_buf_;//when FLAGS_enable_thread_worker=false, we only use this data_buf to read data cyclically

private:
    typedef std::list<osl::MemoryDataStreamPtr> MemoryDataStreamPtrList;
    MemoryDataStreamPtrList data_buf_list_;

    osl::MemoryDataStreamPtr GetFreeDataStream()
    {
        if (data_buf_list_.empty())
        {
            return CreateNewDataBuffer();
        }


#ifdef _TRACE
        {
            MemoryDataStreamPtrList::iterator it(data_buf_list_.begin());
            for (int i = 0; it != data_buf_list_.end(); ++it, ++i)
            {
                TRACE("i=%d buffer->ref=%d", i, (*it)->getRef());
            }
        }
#endif

        //data_buf_list_ is not empty
        MemoryDataStreamPtrList::iterator it(data_buf_list_.begin());
        if ((*it)->getRef() == 1)
        {
            TRACE("Get a free DataBuffer");
            (*it)->reset();
            osl::MemoryDataStreamPtr buf = *it;
            data_buf_list_.erase(it);
            data_buf_list_.push_back(buf);
            return buf;
        }

        TRACE("Need create a new DataBuffer, data_buf_list_.size()=%lu", data_buf_list_.size());
        return CreateNewDataBuffer();
    }

    osl::MemoryDataStreamPtr CreateNewDataBuffer()
    {
        osl::MemoryDataStreamPtr buf = new osl::MemoryDataStream(FLAGS_input_buffer_size);
        data_buf_list_.push_back(buf);
        return buf;
    }
};

#endif

