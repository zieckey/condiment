#ifndef _REDUCE_COMMAND_HANDLER_H_
#define _REDUCE_COMMAND_HANDLER_H_

#include "command_handler.h"

#include "qoslib/include/QOSLibAllExp.h"
#include "qoslib/include/QTokener.h"
#include "qoslib/include/QMD5.h"

#include "constant.h"
#include "common.h"
#include "qlog.h"
#include "mid.h"

class CommandHandlerImpl : public CommandHandler
{

public:

    CommandHandlerImpl();
    virtual bool Work(osl::Slice& command);

    //When finish the job, we do the last flush
    void LastSerialize()
    {
        LastSerialize_aggregate_opt();
    }

private:
    void LastSerialize_aggregate_opt();

private:
#ifdef USING_TOKENER
    osl::Tokener token_;
#endif

    std::string current_mid_;
private:
#ifdef COMPACT_MEMORY
    mid_stringset_map mid_verset_map_;//mid/version_set map
    std::string current_mid_slice_;
    zl::MID mid_;
    size_t mid_len_;
#else
    string_stringset_map mid_verset_map_;//mid/version_set map
#endif
};
namespace qh { namespace str { //{{{ 
    struct less { 
        bool operator()(const char* s1, const char* s2) const
        {
            return strcmp(s1, s2) < 0;
        }
    };
    struct equal_to { 
        bool operator()(const char* s1, const char* s2) const
        {
            return strcmp(s1, s2) == 0;
        }
    };

    struct hash
        : public std::unary_function<const char*, std::size_t>
    {
        std::size_t
            operator()(const char* __s) const
#ifdef __linux__
            { return std::tr1::Fnv_hash<>::hash(__s, strlen(__s)); }
#else
            { return std::tr1::_Fnv_hash<>::hash(__s, strlen(__s)); }
#endif
    };
}//end of namespace str
}//}}} end of namespace qh  

inline void CommandHandlerImpl::LastSerialize_aggregate_opt()
{//{{{
    //step 1 : represents all mids in any versions
#ifdef USING_HASH_MAP
    typedef std::tr1::unordered_map<const char*, 
                                    osl::u32,
                                    qh::str::hash,
                                    qh::str::equal_to > charptru32map;
#else
    typedef std::map<const char*, osl::u32, qh::str::less> charptru32map;
#endif

    charptru32map ver_count_map;
#ifdef USING_HASH_MAP
    ver_count_map.rehash(800);
#endif
    ver_count_map["-1"] = mid_verset_map_.size();

    //step 2 : aggregate now in memory
    {
#ifdef COMPACT_MEMORY
        mid_stringset_map::iterator it(mid_verset_map_.begin());
        mid_stringset_map::iterator ite(mid_verset_map_.end());
#else
        string_stringset_map::iterator it(mid_verset_map_.begin());
        string_stringset_map::iterator ite(mid_verset_map_.end());
#endif
        charptru32map::iterator vc_it = ver_count_map.end();
        for (; it != ite; ++it)
        {
            stringset& verset = it->second;
            stringset::iterator itset(verset.begin());
            stringset::iterator itendset(verset.end());
            for (; itset != itendset; ++itset)
            {
                if ((*itset).size() == 0)
                {
                    continue;
                }
                vc_it = ver_count_map.find((*itset).c_str());
                if (vc_it == ver_count_map.end())
                {
                    ver_count_map[(*itset).c_str()] = 1;
                }
                else
                {
                    vc_it->second += 1;
                }
            }
        }
    }

    //step 3 : output
    {
        charptru32map::iterator it = ver_count_map.begin();
        charptru32map::iterator ite = ver_count_map.end();
        std::string cache_string;
        cache_string.reserve(12);
        for (; it != ite; ++it)
        {
            //qLogTrace(kLogName, "version=\"%s\" count=%u", it->first.c_str(), it->second);
            AddOutput(it->first, strlen(it->first));
            AddOutput("\t", 1);
            cache_string = osl::StringUtil::valueOf(it->second);
            AddOutput(cache_string.c_str(), cache_string.length());
            AddOutput("\n", 1);
        }
    }
}//}}}

/*
inline void CommandHandlerImpl::LastSerialize_output()
{//{{{
    //---------------------------------------------
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
}//}}}

inline void CommandHandlerImpl::LastSerialize_aggregate()
{//{{{
    //step 1 : represents all mids in any versions
    stringu32map ver_count_map;
#ifdef USING_HASH_MAP
    ver_count_map.rehash(800);
#endif
    ver_count_map["-1"] = mid_verset_map_.size();


    //step 2 : aggregate now in memory
    {
        string_stringset_map::iterator it(mid_verset_map_.begin());
        string_stringset_map::iterator ite(mid_verset_map_.end());
        stringu32map::iterator vc_it = ver_count_map.end();
        for (; it != ite; ++it)
        {
            stringset& verset = it->second;
            stringset::iterator itset(verset.begin());
            stringset::iterator itendset(verset.end());
            for (; itset != itendset; ++itset)
            {
                if ((*itset).size() == 0)
                {
                    continue;
                }
                vc_it = ver_count_map.find(*itset);
                if (vc_it == ver_count_map.end())
                {
                    ver_count_map[*itset] = 1;
                }
                else
                {
                    vc_it->second += 1;
                }
            }
        }

        mid_verset_map_.clear();
    }

    //step 3 : output
    {
        stringu32map::iterator it = ver_count_map.begin();
        stringu32map::iterator ite = ver_count_map.end();
        std::string cache_string;
        cache_string.reserve(12);
        for (; it != ite; ++it)
        {
            //qLogTrace(kLogName, "version=\"%s\" count=%u", it->first.c_str(), it->second);
            AddOutput(it->first.c_str(), it->first.length());
            AddOutput("\t", 1);
            cache_string = osl::StringUtil::valueOf(it->second);
            AddOutput(cache_string.c_str(), cache_string.length());
            AddOutput("\n", 1);
        }
    }
}//}}}
*/

#endif //_COMMAND_HANDLER_H_

