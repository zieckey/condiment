
#include <iostream>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
using namespace std;
struct Ansible
    : public boost::enable_shared_from_this<Ansible>
{
    boost::shared_ptr<Ansible> get_shared()
    {
        boost::shared_ptr<Ansible> r(this);
        return r;
    }
    ~Ansible()
    {
        cout << "Destructor, this=" << this << endl;
    }
};

int main(int argc, char* argv[])
{
    boost::shared_ptr<Ansible> a(new Ansible);
    Ansible& r = *a;
    //boost::shared_ptr<Ansible> b = r.get_shared(); //ERROR usage
    boost::shared_ptr<Ansible> c = r.shared_from_this();
    cout << "Reference Number " << a.use_count() 
        //<< " " << b.use_count() 
        << " " << c.use_count() << endl;
    return 0;
}
