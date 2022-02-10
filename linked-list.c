#include <stdio.h>
#include <stdlib.h>

struct node {
    int value;
    struct node *next;
};

struct node *make_node(int value)
{
    struct node *node = malloc(sizeof(struct node));

    node->value = value;
    node->next = NULL;

    return node;
}

void free_node(struct node *n)
{
    while (n->next != NULL) {
        struct node *tmp = n;
        n = n->next;

        free(tmp);
    }

    free(n);
}

void node_put(struct node **n, int value)
{
    struct node *new_node = make_node(value);

    if (*n == NULL) {
        *n = new_node;
    } else {
        struct node *tmp = *n;
        while (tmp->next != NULL) {
            tmp = tmp->next;
        }
        tmp->next = new_node;
    }
}

int main(void)
{
    struct node *head = NULL;

    node_put(&head, 13);
    node_put(&head, 69);
    node_put(&head, 420);

    for (struct node *n = head; n != NULL; n = n->next) {
        printf("%d\n", n->value);
    }

    free_node(head);

    return 0;
}
