#include <iostream>
#include <string>

using namespace std;

struct _Rep_base
{
    std::string::size_type               _M_length ;
    std::string::size_type               _M_capacity ;
    int            _M_refcount ;
};

int main()
{
    cout << "sizeof(_Rep_base)=" << sizeof(_Rep_base) << endl;
    return 1;
}
