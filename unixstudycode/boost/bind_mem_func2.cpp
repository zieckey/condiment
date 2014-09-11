#include <iostream>
#include <vector>
#include <string>
#include "boost/bind.hpp"


class status
{
    std::string name_;
    bool ok_;
public:
    status(const std::string& name): name_(name), ok_(true) {} 

    void break_it()
    {
        ok_ = false;
    }
    bool is_broken() const
    {
        return ok_;
    } void report() const
    {
        std::cout << name_ << " is " << (ok_ ? "working nominally" : "terribly broken") << '\n';
    }
};


int main()
{
    std::vector<status> statuses;
    statuses.push_back(status("status 1"));
    statuses.push_back(status("status 2"));
    statuses.push_back(status("status 3"));
    statuses.push_back(status("status 4"));
    statuses[1].break_it();
    statuses[2].break_it();

    for (std::vector<status>::iterator it = statuses.begin(); it != statuses.end(); ++it)
    {
        it->report();
    }

std::cout <<"\n\n";
    std::for_each(statuses.begin(), statuses.end(), std::mem_fun_ref(&status::report));
std::cout <<"\n\n";

    std::for_each(statuses.begin(), statuses.end(), boost::bind(&status::report, _1));

    return 0;
}


