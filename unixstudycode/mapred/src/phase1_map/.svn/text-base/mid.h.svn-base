#ifndef _MAP_REDUCE_MID_H_
#define _MAP_REDUCE_MID_H_

#include <stdint.h>
#include <tr1/unordered_map>

#include "qoslib/include/QOSLibAllExp.h"
#include "qoslib/include/QMD5.h"
#include "qoslib/include/QBase64.h"


namespace zl
{
    struct MID
    {
        union
        {
            struct 
            {
                uint64_t l_;
                uint64_t h_;
            };
            unsigned char d_[16];
        };

        /*
        MID(const MID& r)
        {
            memcpy(d_, r.d_, sizeof(d_));
        }

        MID& operator=(const MID& r) 
        {
            memcpy(d_, r.d_, sizeof(d_));
            return *this;
        }*/
    };

    struct Version
    {
        union
        {
            //7.6.0.2011
            uint64_t i_;
            struct 
            {
                uint16_t d12_; //2011
                uint8_t  d3_;  //0
                uint8_t  d4_;  //6
                uint8_t  d5_;  //7
            };
        };

        bool has_d12_;
        bool has_d3_;
        bool has_d4_;
        bool has_d5_;
    };

    struct hash_mid
        : public std::unary_function<MID, std::size_t>
        {
            std::size_t
                operator()(const MID& __s) const
#ifdef __linux__
                { return std::tr1::Fnv_hash<>::hash((const char*)__s.d_, sizeof(__s.d_)); }
#else   
                { return std::tr1::_Fnv_hash<>::hash((const char*)__s.d_, sizeof(__s.d_)); }
#endif
        };

    struct equal_to
    {
        bool operator()(const MID& m1, const MID& m2) const
        {
            return (m1.l_ == m2.l_ && m1.h_ == m2.h_);
        }
    };

    struct less
    {
        bool operator()(const MID& m1, const MID& m2) const
        {
            if (m1.l_ < m2.l_)
            {
                return true;
            }
            else if (m1.l_ == m2.l_)
            {
                if (m1.h_ < m2.h_)
                {
                    return true;
                }
            }

            return false;
        }
    };

    inline void Convert(const char* s, size_t len, MID& m)
    {
        if (len < 32)
        {
            memset(m.d_, 0, sizeof(m.d_));
        }
        if (len > 32)
        {
            len = 32;
        }
        osl::MD5::convertHex32ToBinary16(s, len, m.d_);
    }

    inline void Convert(const std::string& s, MID& m)
    {
        if (s.length() < 32)
        {
            memset(m.d_, 0, sizeof(m.d_));
        }
        if (s.length() > 32)
        {
            osl::MD5::convertHex32ToBinary16(s.c_str(), 32, m.d_);
        }
        else
        {
            osl::MD5::convertHex32ToBinary16(s.c_str(), s.size(), m.d_);
        }
    }

    inline MID Convert(const char* s, size_t len)
    {
        MID m;
        Convert(s, len, m);
        return m;
    }

    inline void Convert(const MID& m, char base64s[/*24*/])
    {
        size_t outlen = 24;
        osl::Base64::encode(m.d_, sizeof(m.d_), base64s, outlen);
        assert(outlen == 24);
    }

    inline void Convert(const MID& m, std::string& base64s)
    {
        char buf[28] = {};
        Convert(m, buf);
        base64s = buf;
    }
}//endof namespace zl


#endif


