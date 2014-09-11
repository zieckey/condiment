#include <iostream>
#include <iterator>
#include <vector>
#include <string>
#include "boost/bind.hpp"

class personal_info
{
    std::string name_;
    std::string surname_;
    unsigned int age_;
public:
    personal_info(const std::string& n, const std::string& s, unsigned int age): name_(n), surname_(s), age_(age) {} std::string name() const
    {
        return name_;
    } std::string surname() const
    {
        return surname_;
    }
    unsigned int age() const
    {
        return age_;
    }
};


std::ostream& operator<<(std::ostream& os, const personal_info& pi)
{
    os << pi.name() << ' ' << pi.surname() << ' ' << pi.age() << '\n';
    return os;
}


class personal_info_age_less_than : public std::binary_function< personal_info, personal_info, bool>
{
public:
    bool operator()(const personal_info& p1, const personal_info& p2)
    {
        return p1.age() < p2.age();
    }
};



void f()
{
    std::vector<int> ints;
    ints.push_back(7);
    ints.push_back(4);
    ints.push_back(12);
    ints.push_back(10);
    int count = std::count_if(ints.begin(), ints.end(), 
                                    boost::bind(std::logical_and<bool>(),   
                                                boost::bind(std::greater<int>(), _1, 5), 
                                                boost::bind(std::less_equal<int>(), _1, 10)));
    std::cout << count << '\n';
}

int main()
{
    f();

    std::vector<personal_info> vec;

    vec.push_back(personal_info("Little", "John", 30));
    vec.push_back(personal_info("Friar", "Tuck", 50));
    vec.push_back(personal_info("Robin", "Hood", 40));

    std::sort(vec.begin(), vec.end(), boost::bind(std::less<unsigned int>(), boost::bind(&personal_info::age, _1), boost::bind(&personal_info::age, _2)));
    std::cout << "\n\nsorted by age:\n";
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<personal_info>(std::cout, ""));

    std::sort(vec.begin(), vec.end(), boost::bind(std::less<std::string>(), boost::bind(&personal_info::surname, _1), boost::bind(&personal_info::surname, _2)));
    std::cout << "\n\nsorted by surname:\n";
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<personal_info>(std::cout, ""));

    std::cout << "\n\nsorted by age(personal_info_age_less_than):\n";
    std::sort(vec.begin(), vec.end(), personal_info_age_less_than());
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<personal_info>(std::cout, ""));

    return 0;
}



