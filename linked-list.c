#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct Node {
    int value;
    struct Node *next;
};

struct Node *make_node(int val)
{
    struct Node *n = malloc(sizeof(struct Node));
    assert(n != NULL);

    n->value = val;
    n->next = NULL;

    return n;
}

int main(void)
{
    struct Node *head = make_node(69);

    printf("\n");

    if (a == NULL) {
        printf("Node with value: %i not found\n", value);
    } else {
        printf("%i -- %p\n", a->value, (void *) a->next);
    }

    return 0;
}
