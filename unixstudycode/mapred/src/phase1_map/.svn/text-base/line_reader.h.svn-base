#ifndef _FILE_LINE_READER_H_
#define _FILE_LINE_READER_H_

#include <gflags/gflags.h>

#include "file_reader.h"

DECLARE_int32(line_buffer_len);

class LineReader : public FileReader
{
public:
    LineReader(FILE* fp)
        : FileReader(fp), buf_(new char[FLAGS_line_buffer_len])
    {}

    ~LineReader()
    {
        delete [] buf_;
        buf_ = NULL;
    }

    virtual bool GetLine(osl::Slice& line);

private:
    char* buf_;
};

#endif

