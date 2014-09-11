#include <stdio.h>
#include <unistd.h>


int main(int argc, char *const *argv)
{/*{{{*/
    int pagesize = getpagesize();
    printf("pagesize=%d\n", pagesize);
    return 0;
}/*}}}*/
