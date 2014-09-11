#include <iostream>
#include <string>
#include <tr1/unordered_map>
#include <tr1/unordered_set>

#include "common.h"

using namespace std;
void test1();
void test2();

int main( int argc, char* argv[] )
{
    test1();
    test2();
       
    return 0;
}
void test1()
{
    std::tr1::unordered_map<std::string, int> um;
    std::tr1::unordered_set<std::string> us;
    cout << "sizeof(um)=" << sizeof(um) << endl;
    cout << "sizeof(us)=" << sizeof(us) << endl;

    size_t rehash_count = 64;
    um.rehash(rehash_count);

    string a = "a";
    string a1 = "a";
    string a11 = "a";
    um[a] = 1;
    um[a1] = 11;
    um[a11] = 111;
    um["b"] = 2;
    um["c"] = 3;
    um["d"] = 4;

    cout << "um.size()=" << um.size() << endl;
    cout << "um.count(a)=" << um.count(a) << endl;
    cout << "um.load_factor()=" << um.load_factor() << endl;
    cout << "um.bucket_count()=" << um.bucket_count() << endl;
    cout << "um.max_bucket_count()=" << um.max_bucket_count() << endl;
    
    rehash_count = 100;
    um.rehash(rehash_count);
    cout << "After rehash(" << rehash_count << ") ....\n";
    cout << "um.size()=" << um.size() << endl;
    cout << "um.count(a)=" << um.count(a) << endl;
    cout << "um.load_factor()=" << um.load_factor() << endl;
    cout << "um.bucket_count()=" << um.bucket_count() << endl;
    cout << "um.max_bucket_count()=" << um.max_bucket_count() << endl;

    rehash_count = 4400000;
    um.rehash(rehash_count);
    cout << "After rehash(" << rehash_count << ") ....\n";
    cout << "um.size()=" << um.size() << endl;
    cout << "um.count(a)=" << um.count(a) << endl;
    cout << "um.load_factor()=" << um.load_factor() << endl;
    cout << "um.bucket_count()=" << um.bucket_count() << endl;
    cout << "um.max_bucket_count()=" << um.max_bucket_count() << endl;
 
    rehash_count = 5*1024*1024;
    um.rehash(rehash_count);
    cout << "After rehash(" << rehash_count << ") ....\n";
    cout << "um.size()=" << um.size() << endl;
    cout << "um.count(a)=" << um.count(a) << endl;
    cout << "um.load_factor()=" << um.load_factor() << endl;
    cout << "um.bucket_count()=" << um.bucket_count() << endl;
    cout << "um.max_bucket_count()=" << um.max_bucket_count() << endl;
}

namespace zl
{
    struct equal_to
    {
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
}

void test2()
{
    //std::tr1::unordered_map<const char*, int, std::tr1::hash<const char*>, zl::equal_to > um;
    std::tr1::unordered_map<const char*, int, zl::hash, zl::equal_to > um;
    cout << "sizeof(um)=" << sizeof(um) << endl;

    size_t rehash_count = 64;
    um.rehash(rehash_count);

    string a = "a";
    string a1 = "a";
    string a11 = "a";
    um[a.c_str()] = 1;
    um[a1.c_str()] = 11;
    um[a11.c_str()] = 111;
    um["b"] = 2;
    um["c"] = 3;
    um["d"] = 4;

    TRACE("um.size()=%lu", um.size());
    cout << "um.size()=" << um.size() << endl;
    cout << "um.count(a)=" << um.count(a.c_str()) << endl;
    cout << "um.load_factor()=" << um.load_factor() << endl;
    cout << "um.bucket_count()=" << um.bucket_count() << endl;
    cout << "um.max_bucket_count()=" << um.max_bucket_count() << endl;
    
    rehash_count = 100;
    um.rehash(rehash_count);
    cout << "After rehash(" << rehash_count << ") ....\n";
    cout << "um.size()=" << um.size() << endl;
    cout << "um.count(a)=" << um.count(a.c_str()) << endl;
    cout << "um.load_factor()=" << um.load_factor() << endl;
    cout << "um.bucket_count()=" << um.bucket_count() << endl;
    cout << "um.max_bucket_count()=" << um.max_bucket_count() << endl;

    rehash_count = 4400000;
    um.rehash(rehash_count);
    cout << "After rehash(" << rehash_count << ") ....\n";
    cout << "um.size()=" << um.size() << endl;
    cout << "um.count(a)=" << um.count(a.c_str()) << endl;
    cout << "um.load_factor()=" << um.load_factor() << endl;
    cout << "um.bucket_count()=" << um.bucket_count() << endl;
    cout << "um.max_bucket_count()=" << um.max_bucket_count() << endl;
 
    rehash_count = 5*1024*1024;
    um.rehash(rehash_count);
    cout << "After rehash(" << rehash_count << ") ....\n";
    cout << "um.size()=" << um.size() << endl;
    cout << "um.count(a)=" << um.count(a.c_str()) << endl;
    cout << "um.load_factor()=" << um.load_factor() << endl;
    cout << "um.bucket_count()=" << um.bucket_count() << endl;
    cout << "um.max_bucket_count()=" << um.max_bucket_count() << endl;
}
