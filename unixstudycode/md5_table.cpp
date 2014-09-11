
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;
void test_1();
void test_2();
int main(int argc, char* argv[])
{
    test_1();
    printf("\n\n\n");
    test_2();
}

void test_1()
{
    string validmd5chs = "0123456789abcdefABCDEF";
    for (int i = 0; i < 256; i++)
    {
        bool found = false;
        for (int j = 0; j < validmd5chs.length(); j++)
        {
            if (validmd5chs[j] == i)
            {
                //printf("validmd5chs[j] = %c i=%x\n", validmd5chs[j], i);
                found = true;
                break;
            }
        }
        if (found)
        {
            printf("1, ");
        }
        else
        {
            printf("0, ");
        }
    }
}

void test_2()
{
    string validmd5chs = "0123456789.";
    for (int i = 0; i < 256; i++)
    {
        bool found = false;
        for (int j = 0; j < validmd5chs.length(); j++)
        {
            if (validmd5chs[j] == i)
            {
                //printf("validmd5chs[j] = %c i=%x\n", validmd5chs[j], i);
                found = true;
                break;
            }
        }
        if (found)
        {
            printf("1, ");
        }
        else
        {
            printf("0, ");
        }
    }
}
