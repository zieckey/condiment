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
public:
    CommandHandlerImpl();
    virtual bool Work(osl::Slice& command);
    void LastSerialize();

private:
    /**
     * Get the mid and version from the command
     * @param[in] command
     * @param[out] mid 
     * @param[out] ver 
     * @return true if get successfully
     */
    bool GetMIDVer(osl::Slice& command, osl::Slice& mid, osl::Slice& ver);

#ifdef USING_TOKENER 
    void GetMID(osl::Slice& command, osl::Slice& mid)
    {//{{{
        static const unsigned char valid_md5_chs[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

        size_t current_pos = token_.getCurPos();
        while (1 == valid_md5_chs[(int)token_.next()]) //TODO if (token_.next() > 0)
        {
            ;
        }

        token_.back();

        size_t end_pos = token_.getCurPos();
        mid = osl::Slice(command.data() + current_pos, end_pos - current_pos);//TODO if (end_pos - current_pos) > 0)

#ifdef _DEBUG
        std::string st(command.data() + current_pos, end_pos - current_pos);
        qLogTraces(kLogName) << "find mid='" << st << "'";
#endif
    }//}}}

    void GetVer(osl::Slice& command, osl::Slice& ver)
    {//{{{

        static const char valid_ver_chs[] = {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        };

        size_t current_pos = token_.getCurPos();
        while (1 == valid_ver_chs[(int)token_.next()]) //TODO if (token_.next() > 0)
        {
            ;
        }
        token_.back();

        size_t end_pos = token_.getCurPos();
        ver = osl::Slice(command.data() + current_pos, end_pos - current_pos);//TODO if (end_pos - current_pos) > 0)

#ifdef _DEBUG
        std::string st(command.data() + current_pos, end_pos - current_pos);
        qLogTraces(kLogName) << "find ver='" << st << "'";
#endif
    }//}}}
private:
    osl::Tokener token_;
#endif

private:
#ifdef COMPACT_MEMORY
    mid_stringset_map mid_verset_map_;//mid/version_set map
#else
    string_stringset_map mid_verset_map_;//mid/version_set map
#endif
};

#ifdef COMPACT_MEMORY
inline void CommandHandlerImpl::LastSerialize()
{
    char base64s[24] = {};
    mid_stringset_map::iterator it(mid_verset_map_.begin());
    mid_stringset_map::iterator ite(mid_verset_map_.end());
    for (; it != ite; ++it)
    {
        zl::Convert(it->first, base64s);
        AddOutput(base64s, sizeof(base64s) - 2);//omit 2 byte "=="

#ifdef _TRACE
        TRACE("\"%s\"", std::string(base64s, sizeof(base64s)).c_str());
#endif

        stringset& verset = it->second;
        stringset::iterator itset(verset.begin());
        stringset::iterator itendset(verset.end());
        for (; itset != itendset; ++itset)
        {
            if ((*itset).size() > 0)
            {
                AddOutput("\t", 1);
                AddOutput((*itset).data(), (*itset).size());
            }
        }

        AddOutput("\n", 1);
    }
    //mid_verset_map_.clear();
}
#else
inline void CommandHandlerImpl::LastSerialize()
{
    string_stringset_map::iterator it(mid_verset_map_.begin());
    string_stringset_map::iterator ite(mid_verset_map_.end());
    for (; it != ite; ++it)
    {
        AddOutput(it->first.data(), it->first.size());

        stringset& verset = it->second;
        stringset::iterator itset(verset.begin());
        stringset::iterator itendset(verset.end());
        for (; itset != itendset; ++itset)
        {
            if ((*itset).size() > 0)
            {
                AddOutput("\t", 1);
                AddOutput((*itset).data(), (*itset).size());
            }
        }

        AddOutput("\n", 1);
    }
    //mid_verset_map_.clear();
}
#endif

#endif //_COMMAND_HANDLER_H_

