#ifndef _MAP_REDUCE_COMMON_H_
#define _MAP_REDUCE_COMMON_H_

#include <sys/time.h>

inline void logAll(const char* filename, const char* funcname, int lineno, const char* fmt, ... )
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    struct tm *pTime;
    time_t ctTime;
    //time( &ctTime );
    ctTime = tv.tv_sec;
    pTime = localtime( &ctTime );
    const size_t BUF_SIZE = 1024 * 128;
    char s[BUF_SIZE];
    int writen = snprintf( s, sizeof(s), "%4d/%.2d/%.2d %.2d:%.2d:%.2d %.3ld %s:%d:%s - ", 
                pTime->tm_year + 1900, pTime->tm_mon + 1, pTime->tm_mday,
                pTime->tm_hour, pTime->tm_min, pTime->tm_sec, tv.tv_usec / 1000, filename, lineno, funcname );

    if ( writen <= 0 )
    {
        fprintf( stderr, "snprintf return error, errno=%d\n", errno );
        return;
    }
    va_list ap;
    va_start(ap,fmt);
    int len = vsnprintf(s + writen, sizeof(s) - writen, fmt, ap);
    (void)len;
    va_end(ap);

#ifdef _DEBUG
    fprintf(stdout, "%s\n", s);
#else
    fprintf(stdout, "%s\n", s);
#endif
}

#ifdef _TRACE
    #define TRACE(fmt, args...) logAll(__FILE__, __func__, __LINE__, fmt, ##args )
#else
    #define TRACE(fmt, args...) {}
#endif 

#ifdef _DEBUG
    #define DEBUG(fmt, args...) logAll(__FILE__, __func__, __LINE__, fmt, ##args )
#else
    #define DEBUG(fmt, args...) {}
#endif 

    #define INFO(fmt, args...) logAll(__FILE__, __func__, __LINE__, fmt, ##args )

#endif

