#include <boost/thread/thread.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <iostream> 
class HelloWorld
{
    public:
        void hello()
        {
            std::cout << "Hello world, I'm a worker thread, tid=" << pthread_self() << std::endl; 
        }
        void start()
        {
            boost::function< void() > f =  boost::bind(&HelloWorld::hello,this);
            boost::thread thrd( f );
            thrd.join();
        }

}; 
int main(int argc, char* argv[])
{
    std::cout << "Hello world, I'm the main thread, tid=" << pthread_self() << std::endl; 
    HelloWorld hello;
    hello.start();
    return 0;
} 
