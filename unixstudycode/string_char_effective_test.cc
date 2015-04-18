#include<iostream>
#include<cstdlib>
#include<string>
#include<ctime>
#include<cstring>

using namespace std;

const int array_size = 20000;
const int loop_count = 1000000;

void test_charptr_copy()
{
    size_t start=clock();
    char s1[array_size];
    char* s2= new char[array_size]; 
    memset(s2, 'c', array_size);
    for(int i=0;i!=loop_count;++i)
            strcpy(s1,s2);
    cout<<__func__ << " : " << clock()-start<<endl;
    delete s2;
    s2 = NULL;
}

void test_string_copy()
{
    size_t start=clock();
    string s1;
    string s2;
    s2.append(array_size, 'c');
    for(int i=0;i!=loop_count;++i)
            s1=s2;
    cout<<__func__ << " : " << clock()-start<<endl;
}

const std::string& return_string(int code) {
    static const std::string code_array[] = {
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
            "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb",
            "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc",
            "dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd",
    };

    if (code < 4) {
        return code_array[code];
    }

    static const std::string empty_string = "";
    return empty_string;
}

std::string return_charptr(int code) {
    static const char* code_array[] = {
            "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
            "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb",
            "cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc",
            "dddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd",
    };

    if (code < 4) {
        return code_array[code];
    }

    return "";
}

std::string test_return_charptr() {
    size_t start=clock();
    std::string s;
    for (int i = 0; i < loop_count; ++i) {
        s = return_charptr(i%5);
    }
    cout<<__func__ << " : " << clock()-start<<endl;
    return s;
}

std::string test_return_string() {
    size_t start=clock();
    std::string s;
    for (int i = 0; i < loop_count; ++i) {
        s = return_string(i%5);
    }
    cout<<__func__ << " : " << clock()-start<<endl;
    return s;
}

int main()
{
    test_charptr_copy();
    test_string_copy();
    test_return_charptr();
    test_return_string();
    return EXIT_SUCCESS;
}

/**
 * test result:
 *
 * test_charptr_copy : 3920000
 * test_string_copy : 10000
 * test_return_charptr : 130000
 * test_return_string : 40000
 */
