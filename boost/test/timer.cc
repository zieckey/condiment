

#include "test_common.h"
#include <boost/timer.hpp>

TEST_UNIT(timer_test)
{
    boost::timer t;
    std::cout << t.elapsed() << "s\n";
    std::cout << t.elapsed() << "s\n";
    std::cout << t.elapsed() << "s\n";
    std::cout << t.elapsed() << "s\n";
}




#include <boost/date_time/posix_time/posix_time.hpp>

TEST_UNIT(time_duration_test)
{
    boost::posix_time::time_duration t;
    std::cout << "sizeof(boost::posix_time::time_duration)=" << sizeof(t) << std::endl;
}