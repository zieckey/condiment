#include <stdio.h>

int main()
{
    register int i = 0;
    int* p = NULL;
    for ( i = 0; i < 100; ++i )
    {
        //p = &i; // This statement will course error:address of register variable 'i' requested
        printf("i=%d . NOT allow to take the register variables. \n", i);
    }
}
