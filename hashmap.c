#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

struct Slot {
    char *key;
    char *value;
};

struct Hashtable {
    size_t size;
    size_t cap;
    struct Slot *slots;
};

static size_t djb2(unsigned char *str)
{
    size_t hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

static size_t generate_index(char *k, size_t cap)
{
    size_t hash = djb2((unsigned char *) k);
    size_t index = hash % cap;

    return index;
}

#define START_CAP 8
struct Hashtable *make_hashtable()
{
    struct Hashtable *ht = malloc(sizeof(struct Hashtable));
    assert(ht != NULL);

    ht->size = 0;
    ht->cap = START_CAP;

    struct Slot *slots = calloc(ht->cap, sizeof(struct Slot));
    assert(slots != NULL);

    ht->slots = slots;

    return ht;
}

void free_hashtable(struct Hashtable *ht)
{
    free(ht->slots);
    free(ht);
}

void hashtable_debug(struct Hashtable *ht)
{
    printf("size: %lu\n", ht->size);
    printf("capacity: %lu\n", ht->cap);
    printf("slots:\n");
    for (size_t i = 0; i < ht->cap; ++i) {
        printf("%lu: %s - %s\n", i, ht->slots[i].key, ht->slots[i].value);
    }
}

void hashtable_put(struct Hashtable *ht, char *k, char *v)
{
    assert(ht->size <= ht->cap);

    size_t index = generate_index(k, ht->cap);
    while (ht->slots[index].key != NULL) {
        index = (index + 1) % ht->cap;
    }

    ht->slots[index] = (struct Slot){ .key = k, .value = v };
    ht->size++;
}

char *hashtable_get(struct Hashtable *ht, char *k)
{
    size_t index = generate_index(k, ht->cap);
    size_t start_index = index;

    for (;;) {
        struct Slot slot = ht->slots[index];
        if (slot.key == NULL) return NULL;
        if (strcmp(slot.key, k) == 0) return slot.value;

        index = (index + 1) % ht->cap;
        // made full circle
        if (index == start_index) return NULL;
    }
}

int main(void) {
    struct Hashtable *ht = make_hashtable();
    hashtable_debug(ht);
    printf("-------------------------------------\n");

    hashtable_put(ht, "hello", "world");
    hashtable_put(ht, "how", "are");
    hashtable_put(ht, "you", "what");
    hashtable_put(ht, "are", "you");
    hashtable_put(ht, "doing", "today");
    // @TODO: replace old value
    // hashtable_put(ht, "doing", "change");
    hashtable_debug(ht);
    printf("-------------------------------------\n");

    printf("key: hello, value: %s\n", hashtable_get(ht, "hello"));
    printf("key: you, value: %s\n", hashtable_get(ht, "you"));
    printf("key: doing, value: %s\n", hashtable_get(ht, "doing"));
    printf("key: do_not_exist, value: %s\n", hashtable_get(ht, "do_not_exist"));

    free_hashtable(ht);
    return 0;
}
