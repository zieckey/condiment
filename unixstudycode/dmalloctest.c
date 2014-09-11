/**
 * gcc -DDMALLOC -DDMALLOC_FUNC_CHECK dmalloctest.c -I /home/weizili/bin/include /home/weizili/bin/lib/libdmalloc.a
 * dmalloc -l logfile -i 100 low
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef DMALLOC
#include <dmalloc.h>
#endif

int main(int argc, char **argv)
{

    char* str = NULL;
    str = malloc(5);
    str = malloc(6);
    return 0;
}

