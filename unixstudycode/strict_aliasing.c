#include <stdio.h>

int main()
{
    int a = 0x12345678;
    short *p = (short *)&a;
    short temp;
    temp = *p;
    *p = *(p+1);
    *(p+1) = temp;
    printf("%x\n", a);
    return 0;
}
