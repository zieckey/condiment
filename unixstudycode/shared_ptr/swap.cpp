
#include <iostream> 
#include <tr1/memory> 

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

/* Method swap() : exchange the content of the shared pointers. 
 * not empty 
 * not empty 
 * is empty 
 */ 
int main() 
{

     std::tr1::shared_ptr<std::string> sp1; 
     std::tr1::shared_ptr<std::string> sp2(new std::string("demo"));

      PtrUtil::is_empty(sp1); 
      PtrUtil::is_empty(sp2);


       sp1.swap(sp2);

        PtrUtil::is_empty(sp1); 
        PtrUtil::is_empty(sp2); 


        return 0; 

} 
