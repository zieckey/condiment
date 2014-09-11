#include "command_handler.impl.h"

CommandHandlerImpl::CommandHandlerImpl()
{
}

bool CommandHandlerImpl::Work(osl::Slice& command)
{        
    //qLogTraces(kLogName) << "line:" << command.toString();

    token_.reset(command.data(), command.size());
    std::string ver = token_.nextString();
    osl::Slice count_slice = token_.nextSlice();

    //qLogTrace(kLogName, "ver='%s' count='%s'", ver.c_str(), count_slice.toString().c_str());

    stringmap::iterator itmap = string_map_.end();
    stringmap::iterator itemap = string_map_.end();

    itmap = string_map_.find(ver);
    if (itmap == itemap)
    {
        //qLogTrace(kLogName, "Insert a new version '%s'", ver.c_str());
        string_map_[ver] = atoi(count_slice.data());
    }
    else
    {
        itmap->second += atoi(count_slice.data());
        //qLogTrace(kLogName, "Add %d to version '%s' now count=%u", atoi(count_slice.data()), ver.c_str(), itmap->second);
    }


    return true;
}


