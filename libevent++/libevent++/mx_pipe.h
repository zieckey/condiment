/********************************************************************
 *	Created:	2013/08/01 15:54
 *	File name: 	mx_pipe.h
 *	Author:		weizili
 *	Purpose:	
 *
 *	Copyright 2010-2013, All Rights Reserved.
 ********************************************************************/
#ifndef _MX_PIPE_H_
#define _MX_PIPE_H_

#include "libeventqing/include/inner_pre.h"

#ifdef H_OS_WINDOWS
    #include <stdlib.h>
    #include <stdio.h>
    #include <io.h>
    #include <fcntl.h>
    #include <process.h>
    #include <math.h>
#else
    #include <unistd.h>
#endif

namespace mx
{
    struct PipeFD
    {
        union
        {
            int filedes[2];
            struct  
            {
                int read_fd;
                int write_fd;
            };
        };
    };

    inline int pipe(int filedes[2])
    {
#ifdef H_OS_WINDOWS
        return ::_pipe(filedes, 1024, _O_BINARY);
#else
        return ::pipe(filedes);
#endif
    }

    inline int pipe(PipeFD& pfd)
    {
        return pipe(pfd.filedes);
    }
}

#endif

