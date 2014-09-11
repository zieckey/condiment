#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include <map>
#include <sys/time.h>
#include <ext/hash_map>
#include <tr1/unordered_map>
#include <tr1/unordered_set>

namespace zl
{ //{{{
    struct equal_to
    {
        bool operator()(const char* s1, const char* s2) const
        {
            return strcmp(s1, s2) == 0;
        }
    };
 
    struct hash_string
        : public std::unary_function<std::string, std::size_t>
        {
            std::size_t
                operator()(const std::string& __s) const
#ifdef __linux__
                { return std::tr1::Fnv_hash<>::hash(__s.data(), __s.length()); }
#else
                { return std::tr1::_Fnv_hash<>::hash(__s.data(), __s.length()); }
#endif
        };

    
    struct hash_charptr
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
}//}}}

typedef std::list<std::string> string_list;
typedef std::map<std::string, int> string_map;
typedef __gnu_cxx::hash_map<std::string, int, zl::hash_string> string_hash_map;
typedef std::tr1::unordered_map<std::string, int> string_unordered_map;

void fill_list(string_list& slist, size_t count);
uint64_t current_usec();

int main( int argc, char* argv[] )
{
    if (argc != 2 && argc != 3)
    {
        fprintf(stderr, "Usage:%s test_count rehash\n", argv[0]);
        fprintf(stderr, "For example:%s 10000 rehash\n", argv[0]);
        return -1;
    }

    size_t count = atoi(argv[1]);
    bool rehash = false;
    if (argc == 3)
    {
        rehash = true;
    }

    string_map smap;
    string_hash_map shash_map;
    string_unordered_map sunordered_map;

    if (rehash)
    {
        sunordered_map.rehash(count);
    }

    string_list slist;
    fill_list(slist, count);

    uint64_t start = 0;
    uint64_t end = 0;

    uint64_t map_insert_us = 0;
    uint64_t hash_map_insert_us = 0;
    uint64_t unordered_map_insert_us = 0;

    uint64_t map_traverse_us = 0;
    uint64_t hash_map_traverse_us = 0;
    uint64_t unordered_map_traverse_us = 0;

    uint64_t map_find_us = 0;
    uint64_t hash_map_find_us = 0;
    uint64_t unordered_map_find_us = 0;

    uint64_t map_delete_us = 0;
    uint64_t hash_map_delete_us = 0;
    uint64_t unordered_map_delete_us = 0;



    // Insert test
    {//{{{
        string_list::iterator it(slist.begin());
        string_list::iterator ite(slist.end());

        //map insert
        start = current_usec();
        for (int i = 0; it != ite; ++it, ++i)
        {
            smap[*it] = i;
        }
        end = current_usec();
        map_insert_us = end - start;

        //hash_map insert
        it = slist.begin();
        start = current_usec();
        for (int i = 0; it != ite; ++it, ++i)
        {
            shash_map[*it] = i;
        }
        end = current_usec();
        hash_map_insert_us = end - start;

        //unordered_map insert
        it = slist.begin();
        start = current_usec();
        for (int i = 0; it != ite; ++it, ++i)
        {
            sunordered_map[*it] = i;
        }
        end = current_usec();
        unordered_map_insert_us = end - start;
    }//}}}

    // Traverse test
    {//{{{
        //map traverse 
        {
            string_map::iterator it(smap.begin());
            string_map::iterator ite(smap.end());
            start = current_usec();
            for (int i = 0; it != ite; ++it)
            {
                i++;
            }
            end = current_usec();
            map_traverse_us = end - start;
        }

        //hash_map traverse 
        {
            string_hash_map::iterator it(shash_map.begin());
            string_hash_map::iterator ite(shash_map.end());
            start = current_usec();
            for (int i = 0; it != ite; ++it)
            {
                i++;
            }
            end = current_usec();
            hash_map_traverse_us = end - start;
        }

        //unordered_map traverse 
        {
            string_unordered_map::iterator it(sunordered_map.begin());
            string_unordered_map::iterator ite(sunordered_map.end());
            start = current_usec();
            for (int i = 0; it != ite; ++it)
            {
                i++;
            }
            end = current_usec();
            unordered_map_traverse_us = end - start;
        }
    }//}}}

    // Find test
    {//{{{
        string_list::iterator it(slist.begin());
        string_list::iterator ite(slist.end());

        //map find
        start = current_usec();
        for (int i = 0; it != ite; ++it, ++i)
        {
            smap[*it] = i;
        }
        end = current_usec();
        map_find_us = end - start;

        //hash_map find
        it = slist.begin();
        start = current_usec();
        for (int i = 0; it != ite; ++it, ++i)
        {
            shash_map[*it] = i;
        }
        end = current_usec();
        hash_map_find_us = end - start;

        //unordered_map find
        it = slist.begin();
        start = current_usec();
        for (int i = 0; it != ite; ++it, ++i)
        {
            sunordered_map[*it] = i;
        }
        end = current_usec();
        unordered_map_find_us = end - start;
    }//}}}

    // Delete test
    {//{{{
        string_list::iterator it(slist.begin());
        string_list::iterator ite(slist.end());

        //map delete
        start = current_usec();
        for (int i = 0; it != ite; ++it, ++i)
        {
            smap.erase(*it);
        }
        end = current_usec();
        map_delete_us = end - start;

        //hash_map delete
        it = slist.begin();
        start = current_usec();
        for (int i = 0; it != ite; ++it, ++i)
        {
            shash_map.erase(*it);
        }
        end = current_usec();
        hash_map_delete_us = end - start;

        //unordered_map delete
        it = slist.begin();
        start = current_usec();
        for (int i = 0; it != ite; ++it, ++i)
        {
            sunordered_map.erase(*it);
        }
        end = current_usec();
        unordered_map_delete_us = end - start;
    }//}}}

    //stat output
    std::cout << "          insert, count " << count << std::endl;
    std::cout << "               std::map " << map_insert_us << " us\n";
    std::cout << "      std::ext/hash_map " << hash_map_insert_us << " us\n";
    std::cout << "std::tr1::unordered_map " << unordered_map_insert_us << " us\n";

    std::cout << "\n";
    std::cout << "        traverse, count " << count << std::endl;
    std::cout << "               std::map " << map_traverse_us << " us\n";
    std::cout << "      std::ext/hash_map " << hash_map_traverse_us << " us\n";
    std::cout << "std::tr1::unordered_map " << unordered_map_traverse_us << " us\n";

    std::cout << "\n";
    std::cout << "            find, count " << count << std::endl;
    std::cout << "               std::map " << map_find_us << " us\n";
    std::cout << "      std::ext/hash_map " << hash_map_find_us << " us\n";
    std::cout << "std::tr1::unordered_map " << unordered_map_find_us << " us\n";

    std::cout << "\n";
    std::cout << "          delete, count " << count << std::endl;
    std::cout << "               std::map " << map_delete_us << " us\n";
    std::cout << "      std::ext/hash_map " << hash_map_delete_us << " us\n";
    std::cout << "std::tr1::unordered_map " << unordered_map_delete_us << " us\n";

     
    return 0;
}

void fill_list(string_list& slist, size_t count)
{
    for (size_t i = 0; i < count; ++i)
    {
        std::ostringstream oss;
        oss << i;
        //slist.push_back(MD5::hex(oss.str().c_str(), oss.str().length()));
        slist.push_back(oss.str());//
    }
}


uint64_t current_usec()
{
    struct timeval tv;
    gettimeofday( &tv, NULL );
    return tv.tv_sec * 1000 * 1000 + tv.tv_usec;
}


