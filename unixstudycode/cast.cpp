#include <stdio.h>

#include <sys/time.h>

#include <iomanip>
#include <iostream>
using namespace std;

class A
{
    public:
        A(){ std::cout << "A() called, this=" << this << std::endl;}
        virtual ~A(){ std::cout << "~A() called, this=" << this << std::endl;}
        void print() { std::cout << "class A" << std::endl; }
};

class B : public A
{
    public:
        B(){ std::cout << "B() called, this=" << this << std::endl;}
        ~B(){ std::cout << "~B() called, this=" << this << std::endl;}
        
};

int main()
{
    A* pa = new B();
    void* pv = pa;
    A* pa1 = reinterpret_cast<A*>( pv );
    pa1->print();
    B* pb = dynamic_cast<B*>( pa );
    pb->print();
    delete pb;
    return 0;
}


