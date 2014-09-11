#include <stdio.h>

#include <sys/time.h>

#include <iomanip>
#include <iostream>
using namespace std;


typedef struct 
{
    char a[8];
    char b[8];
    char c[8];
    char d[8];
} item;

const char *item_get_key(const item *item)
{/*{{{*/
    const char* pr = (const char *)(item + sizeof(item));
    printf( "item=%p , sizeof(item)=%lu\n", item, sizeof(item) );
    printf( "item=%p pr=%p,  pr-item=%d\n", item, pr, (int)(pr- (char*)(item)) );
    return pr;
}/*}}}*/


int main()
{
    printf("bool true=%d\n", true );
    printf("sizeof(size_t)=%lu\n", sizeof(size_t));
    printf("sizeof(long long)=%lu\n", sizeof(long long));
    printf("sizeof(long double)=%lu\n", sizeof(long double));
    printf("sizeof(long)=%lu\n", sizeof(long));
    printf("sizeof(time_t)=%lu\n", sizeof(time_t));

    int ia = 0xffeeddcc;
    char c = ia;
    char c1 = (char)ia; (void)c1;
    char c2 = static_cast<char>(ia); (void)c2;
    printf( "%c %u\n", c, (unsigned char)c );


    double fd = 3.3f;
    int id = (int)fd; (void)id;

    int a1 = 1;
    int a0 = 0;

    int b0 = !a0;
    int b1 = !a1;

    printf("a0=%d, a1=%d, b0=%d, b1=%d\n", a0, a1, b0, b1 );

    //--------------------------------------
    printf("sizeof(struct item)=%lu\n", sizeof(item) );
    item it;
    memset( it.a, 'a', sizeof(it.a) );
    memset( it.b, 'b', sizeof(it.b) );
    memset( it.c, 'c', sizeof(it.c) );
    memset( it.d, 'd', sizeof(it.d) );
    const char* p11 = item_get_key( &it );
    printf( "&it=%p p11=%p,  p11-it=%d\n", &it, p11, (int)(p11- (char*)(&it)) );


    //--------------------------------
    uint32_t t32 = 0xfc01;
    printf("t32=%u\n", t32);
    printf("t32=%8x\n", t32);
    std::cout << "cout std::setw(8) << std::setfill('0') t32=";
    std::cout << std::setw(8) << std::setfill('0') << std::hex << t32 << std::endl;
    std::cout << std::setw(8) << std::setfill('0') << 333 << std::endl;


    char buf[4096] = {};
    memset( buf, sizeof(buf) - 1, 'Z' );
    const char* s11 = "hello";
    int writen = snprintf( buf, sizeof(buf), "%s", s11 );
    printf( "writen=%d\n%s\n", writen, buf );


    return 0;
}



