#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

int main()
{
    void** a = malloc((size_t)(64));
    int b = 4;
    for(int i = 0; i < 64 / sizeof(void*); i++)
    {
        a[i] = &b;
    }

    a[6] = (int*)10;

    for(int i = 0; i < 64 / sizeof(void*); i++)
    {
        printf("%li\n", (long int)(int*)a[i]);
    }
    return 0;
}