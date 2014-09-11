
#include <iostream> 
#include <tr1/memory> 
#include "foo.h"
 

/* When sp2 is created, sp1 increments the reference counter. 

 * When the two shared pointer objects get out of scope, the last 
 * one that is destroyed will release the resource. 
 * 
 * output: 
 * foo::print 
 * sp1 pointer: 0x90a7008 
 * foo::print 
 * sp1 pointer: 0x90a7008 
 * sp2 pointer: 0x90a7008 
 * counter sp1: 2 
 * counter sp2: 2 
 */ 
int main() 
{

    std::tr1::shared_ptr<Foo> sp1(new Foo); 
    sp1->print(); 
    std::cout << "sp1 pointer: " << sp1.get() << std::endl;

    std::cout << "counter sp1: " << sp1.use_count() << std::endl; 

    std::tr1::shared_ptr<Foo> sp2(sp1); 
    sp2->print(); 
    std::cout << "counter sp1: " << sp1.use_count() << std::endl; 
    std::cout << "counter sp2: " << sp2.use_count() << std::endl; 

    std::cout << "sp1 pointer: " << sp1.get() << std::endl; 
    std::cout << "sp2 pointer: " << sp2.get() << std::endl;

    std::cout << "counter sp1: " << sp1.use_count() << std::endl; 
    std::cout << "counter sp2: " << sp2.use_count() << std::endl; 


    return 0; 
} 

