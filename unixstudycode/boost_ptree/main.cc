#include <iostream>  
#include <string>  
#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/xml_parser.hpp>  
#include <boost/property_tree/ini_parser.hpp>
#include <boost/foreach.hpp>  

using boost::property_tree::ptree;
using namespace std;

void printTree(ptree& root, int curLayer)
{
    BOOST_FOREACH(ptree::value_type &v1, root){

        if(v1.first == "common"){
            BOOST_FOREACH(ptree::value_type &vAttr, v1.second){

                int layer = curLayer;
                while (layer--)
                {
                    cout<<'\t';
                }

                cout<<vAttr.first<<"="<<vAttr.second.data()<<endl;
            }
        }
        else
        {

#if 0
            int layer = curLayer;
            while (layer--)
            {
                cout<<'\t';
            }

            cout<<v1.first;

            if (v1.second.empty() == true)
            {
                cout<<"="<<v1.second.data();
            }
            cout<<endl;
            if (v1.second.empty() == false)
            {
                printTree(v1.second, curLayer+1);
            }
#endif
        }
    }
}

int main(void)
{  
    ptree pt;  
    boost::property_tree::ini_parser::read_ini("push.ini", pt);

    printTree(pt, 0);
    return 0;
}  
