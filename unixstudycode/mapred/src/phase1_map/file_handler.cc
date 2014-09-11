#include "file_handler.h"

#include <gflags/gflags.h>
#include <qlog.h>

#include "line_reader.h"
#include "buffer_reader.h"

//DEFINE_string(file_reader_type, "BufferReader", "The file reader type, options : LineReader , BufferReader");

bool FileHandler::Init(FILE* fp)
{
/*    if (FLAGS_file_reader_type == "LineReader")
    {
        reader_ = new LineReader(fp);
    }
    else if (FLAGS_file_reader_type == "BufferReader")
    {
        reader_ = new BufferReader(fp);
    }
    else
    {
        return false;
        //qAppErrors(kLogName) << "No such Reader : " << FLAGS_file_reader_type;
    }
*/
    reader_ = new BufferReader(fp);
    return reader_->Init();
}

osl::MemoryDataStreamPtr FileHandler::Read()
{
    return reader_->Read();
    
    /*BufferReader* buffer_reader = dynamic_cast<BufferReader*>(reader_);
    if (buffer_reader)
    {
        return buffer_reader->Read();
    }
    else
    {
        return NULL;
    }
    */
}

