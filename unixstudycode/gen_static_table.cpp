#include <iostream>

int main()
{
    const char* src = ",:]}/\\\"[{;=#";
    char buf[256] = {};
    memset(buf, 1, sizeof(buf));
    for (size_t i = 0; i < strlen(src); ++i)
    {
        buf[ src[i] ] = 0;
    }

    for (size_t i = 0; i < sizeof(buf); ++i)
    {
        if (i < ' ')
        {
            buf[i] = 0;
        }
    }

    for (size_t i = 0; i < sizeof(buf); ++i)
    {
        printf("%d, ", buf[i]);
        if (i % 16 == 15)
        {
            printf("\n");
        }
    }
    return 0;
}
