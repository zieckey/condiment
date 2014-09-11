#ifndef _COMMAND_HANDLER_H_
#define _COMMAND_HANDLER_H_

#include <stdio.h>

#include <string>
#include <vector>
#include <tr1/unordered_set>
#include <tr1/unordered_map>

#include "qoslib/include/QOSLibAllExp.h"

#include "file_writer.h"
#include "buffer_writer.h"
#include "common.h"
#include "mid.h"


class CommandHandler
{
  public:
#ifdef USING_HASH_MAP
    typedef std::tr1::unordered_set<std::string> stringset; //sizeof=48
    typedef std::tr1::unordered_map<std::string, stringset> string_stringset_map;
    typedef std::tr1::unordered_map<std::string, osl::u32> stringu32map;
#else
    typedef std::set<std::string> stringset;
    typedef std::map<std::string, stringset> string_stringset_map;
    typedef std::map<std::string, osl::u32> stringu32map;
#endif


#ifdef USING_HASH_MAP
    typedef std::tr1::unordered_map<zl::MID, stringset, zl::hash_mid, zl::equal_to> mid_stringset_map;
    typedef std::tr1::unordered_map<zl::MID, osl::u32> midu32map;
#else
    typedef std::map<zl::MID, stringset, zl::less> mid_stringset_map;
    typedef std::map<zl::MID, osl::u32> midu32map;
#endif

  public:
    CommandHandler();
    virtual ~CommandHandler();

    bool Init(FILE* fp);

    virtual bool Work(osl::Slice& command) = 0;

    virtual void LastSerialize() = 0;

    bool WorkBuffer(osl::MemoryDataStreamPtr& buf);

    bool Flush();

  protected:

    void AddOutput(const void* data, size_t len)
    {
        writer_->Write(data, len);
    }

    size_t GetMIDLen(const char* sz)
    {//{{{
        static const unsigned char valid_md5_chs[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

        size_t current_pos = 0;
        while (1 == valid_md5_chs[(int)(sz[current_pos])]) //TODO if (token_.next() > 0)
        {
            ++current_pos;
        }

#ifdef _TRACE
        std::string st(sz, current_pos);
        TRACE("find mid='%s'", st.c_str());
#endif
        return current_pos;
    }//}}}

    size_t GetVerLen(const char* sz)
    {//{{{

        static const char valid_ver_chs[] = {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
        };

        size_t current_pos = 0;
        while (1 == valid_ver_chs[(int)(sz[current_pos])]) //TODO if (token_.next() > 0)
        {
            ++current_pos;
        }

#ifdef _TRACE
        std::string st(sz, current_pos);
        TRACE("find ver='%s'", st.c_str());
#endif
        return current_pos;
    }//}}}

  private:

    BufferWriter* writer_;

    class ThreadHandler;
    ThreadHandler* thread_handler_;
};

#endif //_COMMAND_HANDLER_H_

