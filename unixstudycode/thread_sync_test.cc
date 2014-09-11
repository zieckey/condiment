#include <stdio.h>

/**
 * objdump thread_sync_test -d -S      
 *
 */
int main()
{
    int x = 0;
    int y = 5;
    printf("x=y\n");
    x = y;
    printf("x++\n");
    x++;
    printf("++x\n");
    ++x;
    printf("x=1\n");
    x = 2;
    printf("x+=1\n");
    x += 3;
    printf("done\n");

    return 0;
}
