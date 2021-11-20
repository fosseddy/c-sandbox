#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

uint32_t djb2(uint8_t *str)
{
    uint32_t hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

struct Slot {
    char *key;
    void *value;
};

struct Hash_Table {
    size_t size;
    size_t cap;
    struct Slot **slots;
};

static void hashtable_resize(struct Hash_Table *t)
{
    assert(t->cap > 0);

    t->cap *= 2;
    struct Slot **slots = calloc(t->cap, sizeof(struct Slot));
    assert(slots != NULL);

    for (size_t i = 0; i < t->size; ++i) {
        slots[i] = t->slots[i];
    }

    free(t->slots);
    t->slots = slots;
}

struct Hash_Table *make_hashtable()
{
    struct Hash_Table *t = malloc(sizeof(struct Hash_Table));
    assert(t != NULL);

    t->size = 0;
    t->cap = 4;

    t->slots = calloc(t->cap, sizeof(struct Slot));
    assert(t->slots != NULL);

    return t;
}

void free_hashtable(struct Hash_Table *t)
{
    for (size_t i = 0; i < t->cap; ++i) {
        if (t->slots[i] != NULL) {
            free(t->slots[i]);
        }
    }

    free(t->slots);
    free(t);
}

void hashtable_insert(struct Hash_Table *t, const char *k, void *v)
{
    assert(t->size <= t->cap);

    if (t->size == t->cap) {
        hashtable_resize(t);
    }

    uint32_t hash = djb2((uint8_t *) k);
    int index = hash % t->cap;

    while (t->slots[index] != NULL) {
        index = (index + 1) % t->cap;
    }

    struct Slot *s = malloc(sizeof(struct Slot));
    assert(s != NULL);

    s->key = (char *) k;
    s->value = v;

    t->slots[index] = s;
    t->size++;
}

void *hashtable_get(struct Hash_Table *t, const char *k)
{
    uint32_t hash = djb2((uint8_t *) k);
    int index = hash % t->cap;

    struct Slot *s = t->slots[index];

    // @FIXME: infinite loop if slot does not exist
    while (strcmp(s->key, k) != 0) {
        index = (index + 1) % t->cap;
        s = t->slots[index];
    }

    return s->value;
}

int main() {
    struct Hash_Table *t = make_hashtable();
    hashtable_insert(t, "Test1", "1");
    hashtable_insert(t, "Test2", "2");
    hashtable_insert(t, "Test3", "3");
    hashtable_insert(t, "Test4", "4");
    hashtable_insert(t, "Test5", "5");
    hashtable_insert(t, "Test6", "6");
    hashtable_insert(t, "Test7", "7");
    hashtable_insert(t, "Test8", "8");
    hashtable_insert(t, "Test9", "9");
    hashtable_insert(t, "Test10", "10");
    hashtable_insert(t, "Test11", "11");
    hashtable_insert(t, "Test12", "12");
    hashtable_insert(t, "Test13", "13");
    hashtable_insert(t, "Test14", "14");

    free_hashtable(t);
    return 0;
}
