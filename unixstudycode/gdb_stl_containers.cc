
#include <list>
#include <vector>
#include <map>
#include <string>

using namespace std;

struct A
{
    string    s;
    int       i;
    list<int> l;
};

int main()
{
    list<int> li;
    list<string> ls;

    vector<int> vi;
    vector<string> vs;

    map<int, string> m;
    map<string, A> mm;

    for (int i = 1; i <= 300; ++i)
    {
        li.push_back(i);
        char c = (char)(i+'a');
        string s = string(5, c);
        ls.push_back(s);
        
        vi.push_back(i);
        vs.push_back(s);

        m[i] = s;

        A a;
        a.s = s+s;
        a.i = i*2;
        a.l = li;
        mm[s] = a;
    }

    return 0;
}


