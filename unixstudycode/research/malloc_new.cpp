#include <stdio.h>
#include <string>

class A
{
    public:
    A()
    {
        a = "aa";
    }

    std::string a;
};

int main()
{
    for (;;)
    {
        printf("malloc\n");
        char* buf = (char*)malloc(1024*1);
        printf("free\n");
        free(buf);
        printf("new\n");
        A* a = new A();
        printf("delete\n");
        delete a;
        //sleep(5);
    }

    return 0;
}
