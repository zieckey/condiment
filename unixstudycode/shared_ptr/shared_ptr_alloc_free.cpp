
#include <iostream> 
#include <tr1/memory> 
#include "foo.h"

class FooHandler 
{ 

    public: 
        static Foo* alloc() 
        {

             Foo* f = new Foo; 
             std::cout << " a new foo was created" << std::endl; 
             return f; 
        } 

        static void free(Foo* f) 
        { 


            delete f; 
            std::cout << " foo destroyed" << std::endl; 


        } 
}; 

/* 
 * Each time a new object is created or destroyed, a message is printed in the output window (for simplicity, you will ignore the copy 
 * construction or assignment). Function FooHandler::free can be provided as a delete to the shared_ptr constructor. As a result, 
 * when the resource is deleted a message is printed in the output window (you have to run in debugger to see it). 
 * 
 * output: 
 * a new foo was created 
 * foo::print 
 * foo destroyed 
 */ 
int main() 
{

    std::tr1::shared_ptr<Foo> ptr(FooHandler::alloc(), &FooHandler::free); 
    ptr->print(); 

    return 0; 

} 
