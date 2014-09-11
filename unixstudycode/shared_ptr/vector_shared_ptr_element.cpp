
#include <iostream> 
#include <tr1/memory> 
#include <vector> 
#include <algorithm> 

/* The following sample shows a vector of shared_ptr to int; a transformation is applied 
   on the elements of the vector, 

 * doubling the value of the pointed objects. 
 * 
 * The program shows the reference counter to show that calling function double_it() 
 does not affect it, even though this function 
 * returns a shared_ptr by value. 
 */ 
std::tr1::shared_ptr<int> double_it(const std::tr1::shared_ptr<int>& sp) 

{ 

    *sp *= 2; 
    return sp; 


} 

/* 
 * output: 

 * initially 
 * 1 (counter = 1) 
 * 2 (counter = 1) 
 * 3 (counter = 1) 
 * after transformation 
 * 2 (counter = 1) 
 * 4 (counter = 1) 
 * 6 (counter = 1) 
 */ 
int main() 
{

    std::vector<std::tr1::shared_ptr<int> > numbers; 

    numbers.push_back(std::tr1::shared_ptr<int>(new int(1))); 
    numbers.push_back(std::tr1::shared_ptr<int>(new int(2))); 
    numbers.push_back(std::tr1::shared_ptr<int>(new int(3)));


    std::cout << "initially" << std::endl; 
    for (std::vector<std::tr1::shared_ptr<int> >::const_iterator it = numbers.begin(); it != numbers.end(); ++it) 
    {
        std::cout << **it << " (counter = " << (*it).use_count() << ")" << std::endl; 
    }

    std::transform(numbers.begin(), numbers.end(), numbers.begin(), double_it); 
    std::cout << "after transformation" << std::endl; 
    for (std::vector<std::tr1::shared_ptr<int> >::const_iterator it = numbers.begin(); it != numbers.end(); ++it) 
    {
        std::cout << **it << " (counter = " << (*it).use_count() << ")" << std::endl; 
    }

    return 0; 

} 

