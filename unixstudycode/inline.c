
#include <assert.h>

inline int add(int a, int b)
{
    return a+b;
}

int main()
{
    int a = 1;
    int b = 2;
    int c = add(a,b);
    assert(c == 3);
    return 0;
}


