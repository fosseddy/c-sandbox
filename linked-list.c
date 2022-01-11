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

void free_node(struct Node *head)
{
    do {
        struct Node *tmp = head->next;
        free(head);
        head = tmp;
    } while (head != NULL);
}

struct Node *node_get(struct Node *head, int val)
{
    while (head->next != NULL && head->value != val) {
        head = head->next;
    }

    if (head->value != val) {
        return NULL;
    }

    return head;
}

int main(void)
{
    struct Node *head = make_node(69);

    int value = 69;
    struct Node *a = node_get(head, value);

    printf("\n");

    if (a == NULL) {
        printf("Node with value: %i not found\n", value);
    } else {
        printf("%i -- %p\n", a->value, (void *) a->next);
    }

    free_node(head);

    return 0;
}
