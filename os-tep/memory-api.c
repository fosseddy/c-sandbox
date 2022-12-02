#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int *ptr;

    if ((ptr = malloc(sizeof(int) * 100)) == NULL) {
        perror("malloc failed");
    }

    ptr[100] = 69;

    return 0;
}
