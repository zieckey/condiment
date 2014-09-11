#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>


const std::string& f(int a, const std::string& default_value) 
{
    if (a <= 0) {
        return default_value;
    }

    char buf[32];
    snprintf(buf, sizeof(buf), "%d", a);
    static std::string r;
    r = buf;
    return r;
}

int main(int argc, char* argv[])
{
    int a = atoi(argv[1]);

    const std::string& r = f(a, "");

    printf("test OK, r=[%s]!\n", r.data());
    return 0;
}

