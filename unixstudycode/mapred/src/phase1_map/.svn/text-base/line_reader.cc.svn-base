#include "line_reader.h"

#include "qlog.h"
#include "constant.h"

DEFINE_int32(line_buffer_len, 4096, "The max buffer length of one line");

bool LineReader::GetLine(osl::Slice& line)
{
    if (0 != feof(fp_))
    {
        //qLogInfos(kLogName) << "file end!";
        return false;
    }

    if (NULL == fgets(buf_, FLAGS_line_buffer_len, fp_))
    {   
        //qLogInfos(kLogName) << "fgets return NULL!";
        return false;
    }

    //qLogInfos(kLogName) << buf_;
    size_t len = strlen(buf_);
    if (len > 1)
    {
        line = osl::Slice(buf_, len - 1);// not include the ending '\n'
    }
    return true;
}


