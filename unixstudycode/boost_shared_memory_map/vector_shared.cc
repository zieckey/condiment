#include <boost/interprocess/managed_shared_memory.hpp>
#include <cstdlib> //std::system
#include <cstddef>
#include <cassert>
#include <utility>

int main(int argc, char *argv[])
{
   using namespace boost::interprocess;
   typedef std::pair<double, int> doubleintpair;

   if(argc == 1){  //Parent process
      //Remove shared memory on construction and destruction
      struct shm_remove 
      {
         shm_remove() { shared_memory_object::remove("MySharedMemory"); }
         ~shm_remove(){ shared_memory_object::remove("MySharedMemory"); }
      } remover;

      //Construct managed shared memory
      managed_shared_memory segment(create_only, "MySharedMemory", 65536);

      //Create an object of doubleintpair initialized to {0.0, 0}
      doubleintpair *instance = segment.construct<doubleintpair>
         ("doubleintpair instance")  //name of the object
         (0.0, 0);            //ctor first argument

      //Create an array of 10 elements of doubleintpair initialized to {0.0, 0}
      doubleintpair *array = segment.construct<doubleintpair>
         ("doubleintpair array")     //name of the object
         [10]                 //number of elements
         (0.0, 0);            //Same two ctor arguments for all objects

      //Create an array of 3 elements of doubleintpair initializing each one
      //to a different value {0.0, 0}, {1.0, 1}, {2.0, 2}...
      float float_initializer[3] = { 0.0, 1.0, 2.0 };
      int   int_initializer[3]   = { 0, 1, 2 };

      doubleintpair *array_it = segment.construct_it<doubleintpair>
         ("doubleintpair array from it")   //name of the object
         [3]                        //number of elements
         ( &float_initializer[0]    //Iterator for the 1st ctor argument
         , &int_initializer[0]);    //Iterator for the 2nd ctor argument

      //Launch child process
      std::string s(argv[0]); s += " child ";
      if(0 != std::system(s.c_str()))
         return 1;


      //Check child has destroyed all objects
      if(segment.find<doubleintpair>("doubleintpair array").first ||
         segment.find<doubleintpair>("doubleintpair instance").first ||
         segment.find<doubleintpair>("doubleintpair array from it").first)
         return 1;
   }
   else{
      //Open managed shared memory
      managed_shared_memory segment(open_only, "MySharedMemory");

      std::pair<doubleintpair*, std::size_t> res;

      //Find the array
      res = segment.find<doubleintpair> ("doubleintpair array");   
      //Length should be 10
      if(res.second != 10) return 1;

      //Find the object
      res = segment.find<doubleintpair> ("doubleintpair instance");   
      //Length should be 1
      if(res.second != 1) return 1;

      //Find the array constructed from iterators
      res = segment.find<doubleintpair> ("doubleintpair array from it");
      //Length should be 3
      if(res.second != 3) return 1;

      //We're done, delete all the objects
      segment.destroy<doubleintpair>("doubleintpair array");
      segment.destroy<doubleintpair>("doubleintpair instance");
      segment.destroy<doubleintpair>("doubleintpair array from it");
   }
   return 0;
}

