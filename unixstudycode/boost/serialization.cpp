/**
 * @file simple.cc
 * @brief
 *
 * @author garcia.wul (garcia.wul@alibaba-inc.com)
 * @date Jun 24, 2010 2:59:05 PM
 * @version 0.0.0
 *
 */

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

typedef boost::archive::text_iarchive iarchive;
typedef boost::archive::text_oarchive oarchive;

using namespace std;

class Data {
    friend class boost::serialization::access;
    template<class Archive>
        void serialize(Archive& ar, const unsigned int version) {
            cout << __PRETTY_FUNCTION__ << " version=" << version << endl;
            ar& single_data;
            ar& single_str;
        }

    public:
    static void save(const Data& obj, string filename) {
        ofstream stream(filename.c_str());
        oarchive oa(stream);
        oa << obj;
    }

    static void load(Data& obj, string filename) {
        ifstream ifs(filename.c_str());
        iarchive ia(ifs);
        ia >> obj;
    }

    public:
    int single_data;
    string single_str;
};

class Manager {
    friend class boost::serialization::access;
    template<class Archive>
        void serialize(Archive& ar, const unsigned int version) {
            ar& d;
            ar& name;
            ar& title;
        }

    public:
    static void save(const Manager& obj, string filename) {
        ofstream stream(filename.c_str());
        oarchive oa(stream);
        oa << obj;
    }

    static void load(Manager& obj, string filename) {
        ifstream ifs(filename.c_str());
        iarchive ia(ifs);
        ia >> obj;
    }

    public:
    Data   d;
    string name;
    string title;
};

int main(int argc, char const** argv) {
    Data data;
    data.single_data = 1;
    data.single_str = "this is a string";
    Data::save(data, "1.db");
    Data data2;
    Data::load(data2, "1.db");
    cout << data2.single_data << endl;
    cout << data2.single_str << endl;

    cout << endl;

    Manager m;
    m.d.single_data = 1;
    m.d.single_str = "this is a string";
    m.name = "I am manager";
    m.title = "abcdefghijklmnopqrstuvwxyz";
    Manager::save(m, "2.db");
    Manager m2;
    Manager::load(m2, "2.db");
    cout << m.d.single_data << endl;
    cout << m.d.single_str << endl;
    cout << m.name << endl;
    cout << m.title << endl;


    return EXIT_SUCCESS;
}


