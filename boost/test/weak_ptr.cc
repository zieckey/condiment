#include "test_common.h"

#include <boost/smart_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/make_shared.hpp>

using namespace boost;

TEST_UNIT(weak_ptr)
{
    shared_ptr<int> sp(new int(10));
    H_TEST_ASSERT(sp.use_count() == 1);
    weak_ptr<int> wp(sp); //从shared_ptr创建weak_ptr
    H_TEST_ASSERT(wp.use_count() == 1);
    H_TEST_ASSERT(!wp.expired());//判断weak_ptr观察的对象是否失效

    {
        shared_ptr<int> sp2 = wp.lock();//获得一个shared_ptr
        *sp2 = 100;
        H_TEST_ASSERT(wp.use_count() == 2);
    }
     
    {
        shared_ptr<int> sp2(wp);//获得一个shared_ptr
        *sp2 = 200;
        H_TEST_ASSERT(wp.use_count() == 2);
    }

    H_TEST_ASSERT(wp.use_count() == 1);
    H_TEST_ASSERT(*sp.get() == 200);
}



#include <boost/make_shared.hpp>
#include <boost/enable_shared_from_this.hpp>
using namespace boost;
class self_shared : public enable_shared_from_this<self_shared>
{
public:
    self_shared(int n) :x(n){}
    void print()
    {
        std::cout << " self_shared:" << x << std::endl;
    }
    void setx(int v)
    {
        x = v;
    }
private:
    int x;
};

TEST_UNIT(enable_shared_from_this)
{
    shared_ptr<self_shared> sp = make_shared<self_shared>(314);
    H_TEST_ASSERT(sp.use_count() == 1);
    sp->print();
    shared_ptr<self_shared> p = sp->shared_from_this();
    p->setx(1000);
    p->print();
    H_TEST_ASSERT(sp.use_count() == 2);
    H_TEST_ASSERT(p.use_count() == 2);
}