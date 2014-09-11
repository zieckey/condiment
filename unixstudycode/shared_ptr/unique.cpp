
#include <iostream> 
#include <tr1/memory> 

/* Method use_count() returns the number of references to the shared resource (pointed by the current shared pointer object). 
 *
 * Method unique() indicates whether another shared pointed shares the ownership of the same resource or not 
 *
 * (basically, it's identical to 1 == use_count()). 
 * 
 * output: 
 * unique : true 
 * counter : 1 
 * unique : false 
 * counter : 2 
 */ 
int main() 
{

    std::tr1::shared_ptr<std::string> sp1(new std::string("marius bancila")); 
    std::cout << "unique : " << std::boolalpha << sp1.unique() << std::endl; 
    std::cout << "counter : " << sp1.use_count() << std::endl; 
    std::tr1::shared_ptr<std::string> sp2(sp1); 
    std::cout << "unique : " << std::boolalpha << sp1.unique() << std::endl; 
    std::cout << "counter : " << sp1.use_count() << std::endl; 


    return 0; 


} 
