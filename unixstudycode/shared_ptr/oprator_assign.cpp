
#include <iostream> 
#include <tr1/memory> 

#include "foo.h"

/* operator= is overloaded so that a shared poBarer can be assigned from another 

shared_ptr or auto_ptr. 
* 
* output: 

* sp1 = 1 
* sp2 = 2 
* sp1 = 2 
*/ 
int main() 
{

 std::tr1::shared_ptr<Bar> sp1(new Bar(111)); 
std::cout << "sp1 = " << sp1->d() << std::endl;

 std::tr1::shared_ptr<Bar> sp2(new Bar(222)); 
std::cout << "sp2 = " << sp2->d() << std::endl;


 sp1 = sp2; 
std::cout << "sp1 = " << sp1->d() << std::endl; 


return 0; 


} 

