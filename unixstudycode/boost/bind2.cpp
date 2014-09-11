#include <iostream>
#include "boost/bind.hpp"

class some_class
{
public:
    typedef void result_type;
    void print_string(const std::string& s) const
    {
        std::cout << s << '\n';
    }
};

void print_string(const std::string& s)
{
    std::cout << s << '\n';
}

int main()
{
    (boost::bind(&print_string, _1))("Hello func!");
    some_class sc;
    (boost::bind(&some_class::print_string, _1, _2))(sc, "Hello member func!");
}

