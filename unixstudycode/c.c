#include <stdio.h>
#include <stdbool.h>


struct Foo{
    int a;
    int b[];
};      

struct Foo foo1 = { 39, {16, 18, 110} };
struct Foo foo2 = { 36, {150, 60, 140, 9000} };


int main()
{
    printf("bool true=%d\n", true );
    
size_t a = sizeof(struct Foo);
size_t b = sizeof(foo1);
size_t c = sizeof(foo2);
printf("%d\t%d\t%d\n", a, b, c);
    return 0;
}



