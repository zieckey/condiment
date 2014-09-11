#include "command_handler.impl.h"


CommandHandlerImpl::CommandHandlerImpl()
{
#ifdef USING_HASH_MAP
    mid_verset_map_.rehash(4400000);
#endif
}

#ifdef COMPACT_MEMORY
bool CommandHandlerImpl::Work(osl::Slice& command)
{
    osl::Slice mid;
    osl::Slice ver;
    if (!GetMIDVer(command, mid, ver))
    {
        return false;
    }

    TRACE("mid='%s' ver='%s'", mid.toString().c_str(), ver.toString().c_str());
    stringset& verset = mid_verset_map_[zl::Convert(mid.data(), mid.size())];
#ifdef USING_HASH_MAP
    if (verset.size() == 0)
    {
        //first time
        verset.rehash(32);
        TRACE("version_set rehash");
    }
#endif
    verset.insert(std::string(ver.data(), ver.size()));
    return true;
}
#else//
bool CommandHandlerImpl::Work(osl::Slice& command)
{//{{{
    osl::Slice mid;
    osl::Slice ver;
    if (!GetMIDVer(command, mid, ver))
    {
        return false;
    }

    TRACE("mid='%s' ver='%s'", mid.toString().c_str(), ver.toString().c_str());
    stringset& verset = mid_verset_map_[std::string(mid.data(), mid.size())];
#ifdef USING_HASH_MAP
    if (verset.size() == 0)
    {
        //first time
        verset.rehash(32);
        TRACE("version_set rehash");
    }
#endif
    verset.insert(std::string(ver.data(), ver.size()));
    return true;
}//}}}
#endif



#ifdef USING_TOKENER
bool CommandHandlerImpl::GetMIDVer(osl::Slice& command, osl::Slice& mid, osl::Slice& ver)
{//{{{
    token_.reset(command.data(), command.size());
    char ch = 0;
    bool mid_found = false;
    bool ver_found = false;
    while (!token_.isEnd())
    {
        ch = tolower(token_.next());
        if ('?' == ch || '&' == ch) //beginning of the keyword
        {
            //qLogTraces(kLogName) << "find " << ch;
            ch = tolower(token_.next()); 
            if ('m' == ch && !mid_found) // try to get mid
            {
                //qLogTraces(kLogName) << "find " << ch;
                ch = tolower(token_.next());
                if ('=' == ch)//OK find a mid
                {
                    //qLogTraces(kLogName) << "find " << ch << " we try ot get mid";
                    GetMID(command, mid);
                    if (mid.size() > 0)
                    {
                        mid_found = true;
                        if (ver_found)
                        {
                            //qLogTraces(kLogName) << "ver_found=true, break";
                            break;
                        }
                    }
                }
                else if ('i' == ch && 
                          tolower(token_.next()) == 'd' &&
                          token_.next() == '=')
                {
                    //qLogTraces(kLogName) << "find " << ch << " we try ot get mid";
                    GetMID(command, mid);
                    if (mid.size() > 0)
                    {
                        mid_found = true;
                        if (ver_found)
                        {
                            //qLogTraces(kLogName) << "ver_found=true, break";
                            break;
                        }
                    }
                }
            } //end if ('m' == ch && !mid_found)
            else if ('v' == ch && !ver_found) // try to get ver
            {
                if (tolower(token_.next()) == 'e' &&
                    tolower(token_.next()) == 'r' &&
                    token_.next() == '=')
                {
                    //qLogTraces(kLogName) << "find " << ch << " we try ot get ver";
                    GetVer(command, ver);
                    if (ver.size() > 0)
                    {
                        ver_found = true;
                        if (mid_found)
                        {
                            //qLogTraces(kLogName) << "mid_found=true, break";
                            break;
                        }
                    }
                }
            }
        }// end if ('?' == ch || '&' == ch) //beginning of the keyword
    }//end of while

    if (mid.size() > 0)
    {
        return true;
    }

    return false;
}//}}}
#else
bool CommandHandlerImpl::GetMIDVer(osl::Slice& command, osl::Slice& mid, osl::Slice& ver)
{//{{{

#define get_char() (sz[current_pos++])

    size_t current_pos = 0;
    const char* sz = command.data();
    char ch = 0;
    bool mid_found = false;
    bool ver_found = false;
    while (current_pos < command.size())
    {
        ch = sz[current_pos++];
        if ('?' == ch || '&' == ch) //beginning of the keyword
        {
            //qLogTraces(kLogName) << "find " << ch;
            ch = tolower(get_char()); 
            if ('m' == ch && !mid_found) // try to get mid
            {
                //qLogTraces(kLogName) << "find " << ch;
                ch = tolower(get_char());
                if ('=' == ch)//OK find a mid
                {
                    //qLogTraces(kLogName) << "find " << ch << " we try ot get mid";
                    size_t len = GetMIDLen(sz + current_pos);
                    mid.reset(sz + current_pos, len);
                    current_pos += len;
                    if (mid.size() > 0)
                    {
                        mid_found = true;
                        if (ver_found)
                        {
                            //qLogTraces(kLogName) << "ver_found=true, break";
                            break;
                        }
                    }
                }
                else if ('i' == ch && 
                          tolower(get_char()) == 'd' &&
                          get_char() == '=')
                {
                    //qLogTraces(kLogName) << "find " << ch << " we try ot get mid";
                    size_t len = GetMIDLen(sz + current_pos);
                    mid.reset(sz + current_pos, len);
                    current_pos += len;
                    if (mid.size() > 0)
                    {
                        mid_found = true;
                        if (ver_found)
                        {
                            //qLogTraces(kLogName) << "ver_found=true, break";
                            break;
                        }
                    }
                }
            } //end if ('m' == ch && !mid_found)
            else if ('v' == ch && !ver_found) // try to get ver
            {
                if (tolower(get_char()) == 'e' &&
                    tolower(get_char()) == 'r' &&
                    get_char() == '=')
                {
                    //qLogTraces(kLogName) << "find " << ch << " we try ot get ver";
                    size_t len = GetVerLen(sz + current_pos);
                    ver.reset(sz + current_pos, len);
                    current_pos += len;
                    if (ver.size() > 0)
                    {
                        ver_found = true;
                        if (mid_found)
                        {
                            //qLogTraces(kLogName) << "mid_found=true, break";
                            break;
                        }
                    }
                }
            }
        }// end if ('?' == ch || '&' == ch) //beginning of the keyword
    }//end of while

    if (mid.size() > 0)
    {
        return true;
    }

    return false;
}//}}}
#endif

