#include <sys/time.h>

#include<iostream>
#include<cstdlib>
#include<string>
#include<cstring>

using namespace std;

int array_size = 200;
int loop_count = 1000000;

unsigned long utcmicrosecond();

void test_strncpy()
{
    char* s1 = new char[array_size]; 
    char* s2 = new char[array_size]; 
    memset(s2, 'c', array_size);
    unsigned long start = utcmicrosecond();
    for(int i=0;i!=loop_count;++i) strncpy(s1,s2, array_size); 
    cout<<__func__ << " : " << utcmicrosecond()-start<<endl;
    delete s2;
    s2 = NULL;
    delete s1;
    s1 = NULL;
}

void test_string_copy()
{
    string s1;
    string s2;
    s2.append(array_size, 'c');
    unsigned long start = utcmicrosecond();
    for(int i=0;i!=loop_count;++i) s1=s2;
    cout<<__func__ << " : " << utcmicrosecond()-start<<endl;
}

int main(int argc, char* argv[])
{
    if (argc>1) {
        array_size = atoi(argv[1]);
    }
    cout << "array_size=" << array_size << endl;
    test_strncpy();
    test_string_copy();
    return 0;
}

unsigned long utcmicrosecond()
{
    struct timeval tv;
    gettimeofday( &tv, NULL );
    return (unsigned long)(tv.tv_sec * 1000000 + tv.tv_usec);
}

/**
 * test result:
 *
 * test_strncpy : 3400000 
 *  test_string_copy : 10000
 */
