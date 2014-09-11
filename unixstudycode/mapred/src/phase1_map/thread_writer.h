#ifndef _THREAD_WRITER_H_
#define _THREAD_WRITER_H_

#include <stdio.h>

#include "qoslib/include/QOSLibAllExp.h"
#include "qoslib/include/QDataStream.h"

#include "file_writer.h"

class ThreadWriter : public FileWriter
{
    class WThread;

public:
#ifdef _DEBUG
    ThreadWriter(FILE* fp, const std::string& output_path = "Inc_result.tmp");
#else
    ThreadWriter(FILE* fp);
#endif

    virtual ~ThreadWriter();

    virtual bool Init(); 

    virtual bool Write(const void* data, size_t len);

    //called from main thread
    //This will stop the data writing thread
    virtual bool Flush();

protected:
    size_t dump_buffer_max_;

    osl::MemoryDataStreamPtr output_buf_;

    WThread*    thread_;

};

#endif 


