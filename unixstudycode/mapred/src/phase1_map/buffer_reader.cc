#include "buffer_reader.h"

DEFINE_int32(input_buffer_size, 16*1024*1024, "The max buffer size of the input buffer");


bool BufferReader::GetLine(osl::Slice& line)
{
#ifdef _DEBUG
    qLogTraces(kLogName) << "step 1 : check whether there is data to read";
    PrintDebugString();
#endif

    if (data_buf_->getReadableSize() == 0)
    {
        if (0 != feof(fp_))
        {
#ifdef _DEBUG
            qLogTraces(kLogName) << "step 1 result : no more data to read";
#endif
            return false;
        }
    }

#ifdef _DEBUG
    PrintDebugString();
#endif
    size_t readable_size = data_buf_->getReadableSize();
#ifdef _DEBUG
    qLogTraces(kLogName) << "step 2 : try to read one line";
#endif
    const char* line_begin = (char*)data_buf_->getCurReadBuffer();
    const char* line_end = (char*)memchr(line_begin, '\n', readable_size);
    if (line_end)
    {
        line = osl::Slice(line_begin, line_end - line_begin);
        data_buf_->seekg(line.size() + 1);
#ifdef _DEBUG
        qLogTraces(kLogName) 
            << "step 2 result: successfully read one line, readable_size="
            << readable_size 
            << " line_end - line_begin=" << size_t(line_end - line_begin) 
            << " line=\"" << line.toString() << "\"";
        PrintDebugString();
#endif
        return true;
    }

#ifdef _DEBUG
    qLogTraces(kLogName) << "step 3 : cannot find '\n' in the readable data, try to read more data";
    PrintDebugString();
#endif
    if (0 != feof(fp_))
    {   
        line = osl::Slice(line_begin, readable_size);
        data_buf_->reset(); //no more data to read
#ifdef _DEBUG
        qLogTraces(kLogName) << "step 3 result: no more data to read, return all the readable data: " << line.toString();
    PrintDebugString();
#endif
        return true;
    }

#ifdef _DEBUG
    qLogTraces(kLogName) << "step 4 : check whether it is need to expand the buffer.";
#endif
    if (data_buf_->tellg() < (size_t)(FLAGS_input_buffer_size >> 1))
    {
        FLAGS_input_buffer_size *= 2;
        data_buf_->reserve(FLAGS_input_buffer_size);
        line_begin = (char*)data_buf_->getCurReadBuffer();
#ifdef _DEBUG
        qLogTraces(kLogName) << "step 4 result : expand buffer size to " << FLAGS_input_buffer_size; 
    PrintDebugString();
#endif
    }
    
#ifdef _DEBUG
    qLogTraces(kLogName) << "step 5 : move the readable data to the buffer's beginning. Before move data";
    PrintDebugString();
#endif
    data_buf_->reset();//no change to the data which data_buf_->holds
#ifdef _DEBUG
    PrintDebugString();
#endif
    data_buf_->write(line_begin, readable_size);//move to the buf's beginning
#ifdef _DEBUG
    PrintDebugString();
#endif

#ifdef _DEBUG
    qLogTraces(kLogName) << "step 6 : try to read more data. After step 5 move data";
#endif
    int readn = fread((char*)data_buf_->getCache() + readable_size, 1, FLAGS_input_buffer_size - readable_size, fp_);
    if (readn <= 0)
    {
#ifdef _DEBUG
        PrintDebugString();
        qLogTraces(kLogName) << "step 6 no more data to read ";
#endif
        line = osl::Slice(line_begin, readable_size);
        data_buf_->reset(); //no more data to read
        return true;
    }
    data_buf_->seekp(readn);
#ifdef _DEBUG
    PrintDebugString();
    qLogTraces(kLogName) << "step 7 : try to read one line data recursively. After step 6 read data, data_buf:\n" << (char*)data_buf_->getCache();
#endif

    return GetLine(line);
}

osl::MemoryDataStreamPtr BufferReader::Read()
{
    if (0 != feof(fp_))
    {
        DEBUG("step 1 result : no more data to read");
        return NULL;
    }

    osl::MemoryDataStreamPtr buf = GetFreeDataStream();
    buf->reset();
    int readn = fread((char*)buf->getCache(), 1, FLAGS_input_buffer_size, fp_);
    if (readn <= 0)
    {
        DEBUG("step 1 no more data to read ");
        return NULL;
    }

    buf->seekp(readn);
    return buf;
}

