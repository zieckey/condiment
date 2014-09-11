#ifndef _MAP_COMMAND_HANDLER_H_
#define _MAP_COMMAND_HANDLER_H_

#include "command_handler.h"

#include "qoslib/include/QOSLibAllExp.h"
#include "qoslib/include/QTokener.h"
#include "qoslib/include/QMD5.h"

#include "constant.h"
#include "qlog.h"

class CommandHandlerImpl : public CommandHandler
{
    typedef std::map<std::string, osl::u32> stringmap;

public:
    CommandHandlerImpl();
    virtual bool Work(osl::Slice& command);

    void LastSerialize() 
    {
        std::string cache_str;//only for cache
        cache_str.resize(32);

        stringmap::iterator it(string_map_.begin());
        stringmap::iterator ite(string_map_.end());
        for (; it != ite; ++it)
        {
            //qLogTrace(kLogName, "version=\"%s\" count=%u", it->first.c_str(), it->second);
            AddOutput(it->first.c_str(), it->first.length());
            AddOutput("\t", 1);
            cache_str = osl::StringUtil::valueOf(it->second);
            AddOutput(cache_str.c_str(), cache_str.length());
            AddOutput("\n", 1);
        }
    }

private:
    osl::Tokener token_;
    stringmap    string_map_;
};

#endif //_COMMAND_HANDLER_H_

