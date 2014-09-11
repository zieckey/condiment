#include "command_handler.impl.h"


CommandHandlerImpl::CommandHandlerImpl()
{
#ifdef USING_HASH_MAP
    mid_verset_map_.rehash(2473640);
#endif

#ifdef COMPACT_MEMORY
    current_mid_.resize(24);
    current_mid_[22] = '=';
    current_mid_[23] = '=';
    mid_len_ = sizeof(mid_.d_);
#endif
}

#ifdef USING_TOKENER
bool CommandHandlerImpl::Work(osl::Slice& command)
{//{{{
    token_.reset(command.data(), command.size());
    #ifdef COMPACT_MEMORY
    current_mid_slice_ = token_.nextSlice();
    memcpy(&current_mid_[0], current_mid_slice_.data(), current_mid_slice_.size());
    mid_len_ = 18;
    osl::Base64::decode(current_mid_.data(), current_mid_.size(), mid_.d_, mid_len_);
    assert(mid_len_ == 16);
    TRACE("mid=%s", current_mid_.c_str());
    stringset& verset = mid_verset_map_[mid_];
    #else
    current_mid_ = token_.nextString();
    TRACE("mid=%s", current_mid_.c_str());
    stringset& verset = mid_verset_map_[current_mid_];
    #endif
#ifdef USING_HASH_MAP
    if (verset.size() == 0)
    {
        //first time
        verset.rehash(800);
    }
#endif

    while (!token_.isEnd())
    {
#ifdef _TRACE
        std::string ver = token_.nextString();
        verset.insert(ver);
        TRACE("\tver=%s", ver.c_str());
#else
        verset.insert(token_.nextString());
#endif
    }

    return true;
}//}}}
#else
bool CommandHandlerImpl::Work(osl::Slice& command)
{//{{{
    const char* sz = command.data();
    #ifdef COMPACT_MEMORY
    size_t current_pos = 22;
    memcpy(&current_mid_[0], sz, current_pos);
    mid_len_ = 18;
    osl::Base64::decode(current_mid_.data(), 24, mid_.d_, mid_len_);
    assert(mid_len_ == 16);
    TRACE("mid=%s l=%lu h=%lu", current_mid_.c_str(), mid_.l_, mid_.h_);
    stringset& verset = mid_verset_map_[mid_];
    #else
    size_t current_pos = GetMIDLen(sz);
    current_mid_ = std::string(sz, current_pos);
    TRACE("mid=%s", current_mid_.c_str());
    stringset& verset = mid_verset_map_[current_mid_];
    #endif
#ifdef USING_HASH_MAP
    if (verset.size() == 0)
    {
        //first time
        verset.rehash(800);
    }
#endif

    size_t len = 0;
    while (current_pos < command.size())
    {
        if (sz[current_pos] <= ' ')
        {
            ++current_pos;
            continue;
        }

        len = GetVerLen(sz + current_pos);
#ifdef _TRACE
        std::string ver(sz + current_pos, len);
        verset.insert(ver);
        TRACE("\tver=%s", ver.c_str());
#else
        verset.insert(std::string(sz + current_pos, len));
#endif
        current_pos += len + 1;//+1 means we directly skip one char(which is '\t')
    }
    return true;
}//}}}
#endif

/*
bool CommandHandlerImpl::GetMIDVer(osl::Slice& command, osl::Slice& mid, osl::Slice& ver)
{
    token_.reset(command.data(), command.size());

    token_.skipTo('\t');
    size_t _pos = token_.getCurPos();
    mid = osl::Slice(command.data(), _pos);
    ver = osl::Slice(command.data() + _pos + 1, command.size() - mid.size() - 1);

#ifdef _DEBUG
    std::string a(mid.data(), mid.size());
    std::string b(ver.data(), ver.size());
    std::string c(command.data(), command.size() - 1);
    qLogTrace(kLogName, "command  %s", c.data());
    qLogTrace(kLogName, "mid ver '%s'%s'", a.data(), b.data());
#endif
    return true;
}
*/
