#include <unistd.h>
#include <stdio.h>
#include <string.h>

extern char** environ;

int main(int argc, char* argv[])
{
    for (int i = 0; environ[i]; ++i) {
        //printf("i=%d argv[%d]=%p [%s] | environ[%d]=%p [%s]\n", i, i, argv[i], argv[i], i, environ[i], environ[i]);
        printf("environ[%d]=[%s]\n", i, environ[i]);
    }

    printf("\n\n");
    char* g_last = argv[0];
    for (int i = 0; argv[i]; ++i) {
        if (g_last == argv[i]) {
            g_last = argv[i] + strlen(argv[i]) + 1;
        }
    }

    printf(" argv_last=%p\n", g_last);
    printf("environ[0]=%p\n", environ[0]);

    return 0;
}

