#include <boost/thread/thread.hpp> 
#include <iostream> 

void hello() 
{ 
    std::cout << "Hello world, I'm a worker thread, tid=" << pthread_self() << std::endl; 
} 

int main(int argc, char* argv[]) 
{ 
    std::cout << "Hello world, I'm the main thread, tid=" << pthread_self() << std::endl; 
    boost::thread thrd(&hello); 
    thrd.join(); 
    return 0; 
} 
