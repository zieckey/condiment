#include <stdio.h>

int main()
{
    FILE* fp = fopen("a.log", "w+");
    int i = 1;
    for (; i > 0; ++i)
    {
        fprintf( fp, "i=%10d\n", i);
        fflush(fp);
        usleep(1000);
    }

    fclose(fp);
    return 0;
}
