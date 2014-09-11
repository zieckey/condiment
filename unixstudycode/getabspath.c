#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/param.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>

int main( int argc, char* argv[] )
{

#ifdef __FreeBSD__    
    int mib[4];
    mib[0] = CTL_KERN;
    mib[1] = KERN_PROC;
    mib[2] = KERN_PROC_PATHNAME;
    mib[3] = -1;
    char buf[1024];
    size_t cb = sizeof(buf);
    int pid = getpid();
    int ret = sysctl(mib, 4, buf, &cb, NULL, (size_t)0);

    if ( 0 == ret )
    {
        printf( "path:%s\n", buf );
    }
    else
    {
        int en = errno;
        perror( "sysctl call failed" );
        printf( "sysctl call failed : errno=%d pid=%d\n", en, pid );
    }
    sleep( 120 );
#endif


    return 0;
}

/*
//psksvp@ccs.neu.edu

//-------------------

#ifndef __SELF_EXETABLE_PATH__

#define __SELF_EXETABLE_PATH__



#ifdef WIN32

#include <windows.h>

#define PATH_MAX 255

#elif OSX

#include <mach-o/dyld.h>

#include <limits.h>

#elif LINUX

#include <stdio.h>

#include <stdlib.h>

#include <limits.h>

#include <unistd.h>

#include <sys/types.h>

#elif FREEBSD

#include <stdio.h>

#include <stdlib.h>

#include <limits.h>

#include <unistd.h>

#include <sys/types.h>

#else

#error error while compiling SelfExecutablePath.h, please define one of WIN32, OSX, LINUX, FREEBSD

#endif



namespace psksvp

{



    char exepath[PATH_MAX]; // not thread safe

    inline const char* SelfExecutablePath(void)

    {



#ifdef WIN32

        ::GetModuleFileName(NULL, &exepath[0], PATH_MAX);

#elif OSX

        ::_NSGetExecutablePath(exepath, PATH_MAX);

#elif LINUX

        char temp[PATH_MAX];

        ::snprintf(temp, sizeof(temp),"/proc/%d/exe", ::getpid());

        ::realpath(temp, exepath);

#elif FREEBSD

        char temp[PATH_MAX];

        ::snprintf(temp, sizeof(temp),"/proc/%d/file", ::getpid());

        ::realpath(temp, exepath);

#else

#error error while compiling SelfExecutablePath.h, please define one of WIN32, OSX, LINUX, FREEBSD

#endif



        return exepath;

    }



} // namespace psksvp



#endif
*/



