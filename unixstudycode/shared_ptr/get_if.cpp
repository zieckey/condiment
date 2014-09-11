
#include <iostream> 


#include <tr1/memory> 

/* Class shared_ptr defines a bool operator that allows shared pointers to be used 
 * in boolean expressions. 
 *
 *  With auto_ptr, that is not possible; you have to use function get() to access the 
 * internal pointer and check it against NULL. 
 */ 

class PtrUtil 

{ 

    public: 
        static void is_empty(std::tr1::shared_ptr<std::string> ptr) 
        { 

            if (ptr) 
              std::cout << "not empty" << std::endl; 


            else

               std::cout << "is empty" << std::endl; 

        } 
}; 

/* 
 *  output: 
 *
 *  is empty 
 *  not empty 
 */ 
int main() 
{

     std::tr1::shared_ptr<std::string> sp1; 
     std::tr1::shared_ptr<std::string> sp2(new std::string("demo"));


      PtrUtil::is_empty(sp1); 
      PtrUtil::is_empty(sp2); 

      return 0; 

} 

