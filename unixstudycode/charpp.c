#include <stdio.h>

int main()
{
    char *p = "linux";
    char **q = &p;
    *q = "abcxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    *q = (char*)1;

    printf("p=%s\n", p);
    return 0;
}
