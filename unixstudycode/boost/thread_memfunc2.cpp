#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <string>
#include <iostream> 
class HelloWorld
{
    public:
        void hello(const std::string& str)
        {
            std::cout << "Hello world, I'm a worker thread, tid=" << pthread_self() << " parameter=[" << str << "]" << std::endl; 
        }
}; 

int main(int argc, char* argv[])
{ 
    std::cout << "Hello world, I'm the main thread, tid=" << pthread_self() << std::endl; 
    HelloWorld obj;
    boost::thread thrd( boost::bind(&HelloWorld::hello, &obj, "Hello world, I''m a thread!" ) ) ;
    thrd.join();
    return 0;
}
