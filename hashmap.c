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

struct Hash_Map {
    size_t size;
    size_t cap;
    struct Slot **slots;
};

static void hashmap_resize(struct Hash_Map *t)
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

struct Hash_Map *make_hashmap()
{
    struct Hash_Map *t = malloc(sizeof(struct Hash_Map));
    assert(t != NULL);

    t->size = 0;
    t->cap = 4;

    t->slots = calloc(t->cap, sizeof(struct Slot));
    assert(t->slots != NULL);

    return t;
}

void free_hashmap(struct Hash_Map *t)
{
    for (size_t i = 0; i < t->cap; ++i) {
        // @TODO: delete printf()'s
        if (t->slots[i] != NULL) {
            printf("%s -- %s\n", t->slots[i]->key, (char *) t->slots[i]->value);
            free(t->slots[i]);
        } else {
            printf("%p\n", (void *) t->slots[i]);
        }
    }

    free(t->slots);
    free(t);
}

void hashmap_insert(struct Hash_Map *t, const char *k, void *v)
{
    assert(t->size <= t->cap);

    if (t->size == t->cap) {
        hashmap_resize(t);
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

void *hashmap_get(struct Hash_Map *t, const char *k)
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
    struct Hash_Map *t = make_hashmap();
    hashmap_insert(t, "Test1", "1");
    hashmap_insert(t, "Test2", "2");
    hashmap_insert(t, "Test3", "3");
    hashmap_insert(t, "Test4", "4");
    hashmap_insert(t, "Test5", "5");
    hashmap_insert(t, "Test6", "6");
    hashmap_insert(t, "Test7", "7");
    hashmap_insert(t, "Test8", "8");
    hashmap_insert(t, "Test9", "9");
    hashmap_insert(t, "Test10", "10");
    hashmap_insert(t, "Test11", "11");
    hashmap_insert(t, "Test12", "12");
    hashmap_insert(t, "Test13", "13");
    hashmap_insert(t, "Test14", "14");
    hashmap_insert(t, "Test15", "15");
    hashmap_insert(t, "Test16", "16");
    hashmap_insert(t, "Test17", "17");
    hashmap_insert(t, "Test18", "18");

    free_hashmap(t);
    return 0;
}
