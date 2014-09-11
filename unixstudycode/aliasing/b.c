#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int foo;
    int bar;
} item_t;

int return_item (void **item);

int return_item (void **item)
{
    void *mem;

    mem = malloc (1);
    printf ("malloc  ret=%p\n", mem);
    if (mem) {
        *item = mem;
        return 0;
    }
    else
        return 1;
}

int main (int argc, char *argv[])
{
    item_t *item;

    if (return_item ((void **)&item) == 0) {
        printf ("derect conv=%p\n", item);
        free (item);
    }

    return 0;
}
