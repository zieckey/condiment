#ifndef _MAP_COMMAND_HANDLER_H_
#define _MAP_COMMAND_HANDLER_H_

#include "command_handler.h"

#include "qoslib/include/QOSLibAllExp.h"
#include "qoslib/include/QTokener.h"
#include "qoslib/include/QMD5.h"

#include "constant.h"
#include "common.h"
#include "qlog.h"

class CommandHandlerImpl : public CommandHandler
{
    typedef std::vector<osl::Slice> slicevector;
    typedef std::map<std::string, osl::u32> stringmap;

public:
    CommandHandlerImpl();
    virtual bool Work(osl::Slice& command);

    virtual void LastSerialize() 
    {
        stringmap::iterator it(string_map_.begin());
        stringmap::iterator ite(string_map_.end());
        for (; it != ite; ++it)
        {
            //qLogTrace(kLogName, "version=\"%s\" count=%u", it->first.c_str(), it->second);
            AddOutput(it->first.c_str(), it->first.length());
            AddOutput("\t", 1);
            cache_string_ = osl::StringUtil::valueOf(it->second);
            AddOutput(cache_string_.c_str(), cache_string_.length());
            AddOutput("\n", 1);
        }
    }

private:
    osl::Tokener token_;
    slicevector  ver_vector_;
    //stringvector  ver_vector_;
    stringmap    string_map_;
    std::string  all_version_;
    std::string  cache_string_; //only for cache

    osl::Slice   current_mid_; //
    std::string  last_mid_; //
};

#endif //_COMMAND_HANDLER_H_

