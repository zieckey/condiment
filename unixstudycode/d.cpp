#include <stdio.h>
#include <stdbool.h>

int main()
{
    printf("bool true=%d\n", true );
    
    float f = 1.0f;
    int a = int(f);//这里是C++强制转换方式
    int b = *(int*)((float*)&f);
    //int c = *(static_cast<int*>(&f));
    int d = *(reinterpret_cast<int*>(&f));
    printf( "%d\t%d\t%d\n", a, b, d );

    return 0;
}



