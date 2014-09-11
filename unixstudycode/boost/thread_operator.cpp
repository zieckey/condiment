#include <boost/thread/thread.hpp> 
#include <boost/thread/mutex.hpp> 
#include <iostream> 

boost::mutex io_mutex; 

struct count 
{ 
    count(int id) : id(id) { } 

    void operator()() 
    { 
        for (int i = 0; i < 100; ++i) 
        { 
            std::cout << "tid=" << pthread_self() << " work id=" << id << ": " << i << std::endl; 
            usleep(1);
        } 
    } 

    int id; 
}; 

int main(int argc, char* argv[]) 
{ 
    boost::thread thrd1(count(1)); 
    boost::thread thrd2(count(2)); 
    boost::thread thrd3(count(3)); 
    boost::thread thrd4(count(4)); 
    boost::thread thrd5(count(5)); 
    thrd1.join(); 
    thrd2.join(); 
    thrd3.join(); 
    thrd4.join(); 
    thrd5.join(); 
    return 0; 
} 
