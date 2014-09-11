#include "buffer_writer.h"
#include <fstream>
#include <gflags/gflags.h>
#include "qoslib/include/QTimer.h"

DEFINE_int32(dump_buffer_size, 16*1024*1024, "The max buffer size of the output buffer size");

BufferWriter::BufferWriter(FILE* fp) 
    //: FileWriter(fp) 
    : fp_(fp),
    output_buf_(new char[FLAGS_dump_buffer_size]),
    write_pos_(0)
{

}

bool BufferWriter::Write(const void* data, size_t len)
{
    if (write_pos_ + len > (size_t)FLAGS_dump_buffer_size)
    {
        Flush();
    }
    memcpy(output_buf_ + write_pos_, data, len);
    write_pos_ += len;

    return true;
}

bool BufferWriter::Flush()
{//{{{
#ifdef _DEBUG
    {
        static int64_t newid = 0;
        std::string outfile_tmp = GetTempOutputFilePath(newid++);
        std::fstream ofp(outfile_tmp.c_str(), std::ios::out|std::ios::binary|std::ios::app);
        ofp.write((char*)output_buf_, write_pos_);
        ofp.flush();
        ofp.close();
    }
#endif

    fwrite(output_buf_, 1, write_pos_, fp_);
    write_pos_ = 0;
    return true;
}//}}}

namespace
{//{{{
    //return the time string with micro second
    std::string GetUTimeString()
    {
        std::string::value_type szTime[128] = {};
        struct tm *pTime;
        time_t ctTime;
        time( &ctTime );
        pTime = localtime( &ctTime );


        long uunit = 1000 * 1000;
        snprintf( szTime, sizeof(szTime), "%4d%.2d%.2d%.2d%.2d%.2d_%.6ld", 
                    pTime->tm_year + 1900, pTime->tm_mon + 1, pTime->tm_mday,
                    pTime->tm_hour, pTime->tm_min, pTime->tm_sec, 
                    ((long)(s_pTimer->getImmediateSeconds() * uunit))%uunit);

        return std::string(szTime);
    }

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
}//}}}

std::string BufferWriter::GetTempOutputFilePath(int64_t file_id)
{
    char buf[512] = {};
    snprintf(buf, sizeof(buf), "Inc_%ld_%s_%lu.tmp", 
                file_id, GetUTimeString().c_str(), write_pos_);	

    return buf;
}

