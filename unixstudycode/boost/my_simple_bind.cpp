#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>
#include <iterator>
#include <functional>

#include <boost/bind.hpp>

typedef std::vector<int> intvector;

template <typename R, typename T, typename Arg>
class simple_bind_t
{
    typedef R(T::*fn)(Arg&);
    fn fn_;
    T& t_;
public:
    simple_bind_t(fn f, T& t): fn_(f), t_(t) {}

    R operator()(Arg& a)
    {
        return (t_.*fn_)(a);
    }
};

namespace
{
    class placeholder {};
    placeholder _simple_ph1;
}

template <typename R, typename T, typename Arg>
inline simple_bind_t<R, T, Arg> simple_bind(R (T::*fn)(Arg&), T& t, const placeholder&)
{
    return simple_bind_t<R, T, Arg>(fn, t);
}



class Test
{
public:
    void print(int& v)
    {
        std::cout << v << " ";
    }

    void do_stuff(intvector& v)
    {
        std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
        //std::for_each(v.begin(), v.end(), boost::bind(&Test::print, _1, _2));
        //std::for_each(v.begin(), v.end(), std::mem_fun_ref(&Test::print));
    }
};

int main()
{
    Test t;
    intvector vec;
    vec.push_back(42);
    vec.push_back(43);
    vec.push_back(62);
    simple_bind_t< void, Test, intvector > f = simple_bind(&Test::do_stuff, t, _simple_ph1);
    f(vec);

    return 0;
}

