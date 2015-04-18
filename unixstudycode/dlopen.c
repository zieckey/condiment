#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main(int argc, char **argv) {
    void *handle;
    double (*cosine)(double);
    char *error;

    handle = dlopen ("libm.so", RTLD_LAZY);
    if (!handle) {
        fprintf (stderr, "%s:%d %s\n", __FILE__, __LINE__, dlerror());
        exit(1);
    }

    dlerror();    /* Clear any existing error */
    cosine = dlsym(handle, "cos");
    if ((error = dlerror()) != NULL)  {
        fprintf (stderr, "%s:%d %s\n", __FILE__, __LINE__, error);
        exit(1);
    }

    fprintf (stdout, "%s:%d func=%p\n", __FILE__, __LINE__, cosine);
    fprintf (stdout, "%s:%d %f\n", __FILE__, __LINE__, (*cosine)(2.0));
    dlclose(handle);
    return 0;
}
