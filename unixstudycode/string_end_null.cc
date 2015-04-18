#include <assert.h>
#include <iostream>
#include <string>

using namespace std;

test_trim0_1() 
{
    std::string s = "abcde";
    s[0] = '\0';
    s[4] = '\0';
    char ch = '\0';
    cout << "&s[0]=" << (void*)&s[0] << endl;
    s.erase( s.find_last_not_of(ch) + 1 );    // trim right
    cout << "&s[0]=" << (void*)&s[0] << endl;
    s.erase( 0, s.find_first_not_of(ch) );    // trim left
    cout << "&s[0]=" << (void*)&s[0] << endl;
    assert(s == "bcd");
}

int main()
{
    test_trim0_1();
    std::string s;
    s.append("0", 1);
    if (0 == (char)*(&s[0]+1)) {
        cout << "has ending '\\0'";
    }  else {
        cout << "no ending '\\0'";
    }
    return 0;
}

