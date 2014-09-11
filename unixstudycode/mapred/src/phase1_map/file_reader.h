#ifndef _FILE_READER_H_
#define _FILE_READER_H_

#include <stdio.h>

#include "qoslib/include/QOSLibAllExp.h"

class FileReader
{
public:
    FileReader(FILE* fp) 
        : fp_(fp) 
    {
    }

    virtual ~FileReader() 
    { 
        fp_ = NULL; 
    }

    virtual bool Init() { return true; }

    //get a line : without '\n'
    virtual bool GetLine(osl::Slice& line) = 0;

protected:
    FILE* fp_;
};

#endif 


