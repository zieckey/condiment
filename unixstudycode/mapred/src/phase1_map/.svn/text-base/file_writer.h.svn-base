#ifndef _FILE_WRITER_H_
#define _FILE_WRITER_H_

#include <stdio.h>

#include "qoslib/include/QOSLibAllExp.h"

class FileWriter
{
public:
    FileWriter(FILE* fp) 
        : fp_(fp) 
    {
    }

    virtual ~FileWriter() 
    { 
        fp_ = NULL; 
    }

    virtual bool Init() { return true; }

    virtual bool Write(const void* data, size_t len) = 0;

    virtual bool Flush() = 0;

protected:
    FILE* fp_;
};

#endif 


