#include <iostream>
#include <new>

using namespace std;

struct A
{
    A() {
        cout << "called constructor:  A::A()\n";
    }

    ~A() {
        cout << "called destructor :  A::~A()\n";
    }
    union {
        struct {
            char a;
            char b;
            char c;
            char d;
        };
        int x;
    }u;
};

int main()
{
    char buf[1024];
 
    A* p = new (buf) A;
    cout << "buf=" << (void*)buf << endl;
    cout << "  p=" << (void*)p << endl;

    p->u.x = 0x31323334;
    cout << "buf=" << buf << endl;

    //delete p;
    //TODO how delete p : 
    //p->~A();
    cout << "buf=" << buf << endl;

    return 0;
}
