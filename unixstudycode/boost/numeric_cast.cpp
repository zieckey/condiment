
#include <iostream>
#include "boost/cast.hpp"
#include "boost/limits.hpp"
int main()
{
    std::cout << "larger_to_smaller example\n";  // 没有使用numeric_cast的转换
    long l = std::numeric_limits<short>::max();
    short s = l;
    std::cout << "s is: " << s << '\n';
    s = ++l;
    std::cout << "s is: " << s << "\n\n";  // 使用numeric_cas

    try
    {
        l = std::numeric_limits<short>::max();
        s = boost::numeric_cast<short>(l);
        std::cout << "s is: " << s << '\n';
        s = boost::numeric_cast<short>(++l);
        std::cout << "s is: " << s << '\n';
    }
    catch (boost::bad_numeric_cast& e)
    {
        std::cout << e.what() << '\n';
    }

    return 0;
}
