
#include <iostream> 
#include <tr1/memory> 

#include "foo.h"

/* The next sample shows a shared_ptr created from an auto_ptr object. The auto pointer 
   gives up the ownership of the resource, 

 * resetting its wrapped pointer to NULL. 
 * 
 * output: 
 * foo::print 
 * ap1 pointer: 0x99b8008 
 * foo::print 
 * ap1 pointer: 0 
 * sp1 pointer: 0x99b8008 
 */ 
int main() 
{

    std::auto_ptr<Foo> ap1(new Foo); 
    ap1->print(); 
    std::cout << "ap1 pointer: " << ap1.get() << std::endl;


    std::tr1::shared_ptr<Foo> sp1(ap1);
    sp1->print(); 
    std::cout << "ap1 pointer: " << ap1.get() << std::endl; 
    std::cout << "sp1 pointer: " << sp1.get() << std::endl; 


        return 0; 

} 
