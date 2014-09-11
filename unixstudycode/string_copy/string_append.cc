#include <assert.h>

#include <iostream>
#include <string>

using namespace std;

int main()
{
    string a = "0";
    cout << "a.data() =" << (void*)a.data() << endl;
    a.append("1", 1);
    cout << "a.data() =" << (void*)a.data() << endl;
    return 0;
}

