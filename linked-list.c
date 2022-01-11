#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(void)
{
    printf("\n");

    if (a == NULL) {
        printf("Node with value: %i not found\n", value);
    } else {
        printf("%i -- %p\n", a->value, (void *) a->next);
    }

    return 0;
}
