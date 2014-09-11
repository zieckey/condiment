#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>


void test_c_str();
void test_resize();
void test_istringstream();
void test_hex();
void test_null_string();
void test_string_length();

int main()
{
    test_string_length();
    test_null_string();
    test_c_str();
    test_resize();
    test_istringstream();
    test_hex();
    return 0;
}

void test_null_string()
{
    const char* a = NULL;
    (void)a;
    /*
    std::string s(a); //Will core dump
    printf("address a=%p\n", a);
    printf("address s.data()=%p\n", s.data());
    printf("address s.c_str()=%p\n", s.c_str());
    */
}


void test_c_str()
{
    const char* a = "aaaaaaaa";
    std::string s(a, 2);
    printf("address a=%p\n", a);
    printf("address s.data()=%p\n", s.data());
    printf("address s.c_str()=%p\n", s.c_str());
}

void test_resize()
{
    std::string s1;
    s1.resize(8);
    memset(&s1[0], 'a', 8);
    printf("address s1.c_str()=%p\n", s1.c_str());
    printf("contents s1.c_str()=|%s|\n", s1.c_str());
    printf("s1.length()=%lu\n", s1.length());


    s1.resize(6);
    memset(&s1[0], 'a', 6);
    printf("address s1.c_str()=%p\n", s1.c_str());
    printf("contents s1.c_str()=|%s|\n", s1.c_str());
    printf("s1.length()=%lu\n", s1.length());
}

void test_istringstream()
{
    std::string s = "12 34 56";
    std::istringstream iss(s);
    int a, b, c;
    iss >> a >> b >> c;

    std::cout << "a=" << a << std::endl;
    std::cout << "b=" << b << std::endl;
    std::cout << "c=" << c << std::endl;
}

void test_hex()
{
    
    char hex33[33] = {};
    sprintf( hex33, "%02x", 0xA8 );
    std::cout << "hex33=" << hex33 << std::endl;
}


void test_string_length()
{
    std::string s = "11212";
    size_t len = s.length();
    size_t sz = s.size();
    std::cout << "len=" << len << std::endl
        << "size=" << sz << std::endl;
}



