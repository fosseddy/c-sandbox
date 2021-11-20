#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>

static uint32_t djb2(uint8_t *str)
{
    uint32_t hash = 5381;
    size_t c;

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
    struct Slot *slots;
};

static void hashmap_debug(struct Hash_Map *hm)
{
    for (size_t i = 0; i < hm->cap; ++i) {
        if (hm->slots[i].key != NULL) {
            printf("%s -- %s\n", hm->slots[i].key, (char *) hm->slots[i].value);
        } else {
            printf("%p\n", hm->slots[i].key);
        }
    }
}

static void hashmap_resize(struct Hash_Map *hm)
{
    assert(hm->cap > 0);

    hm->cap *= 2;

    struct Slot *slots = calloc(hm->cap, sizeof(struct Slot));
    assert(slots != NULL);

    for (size_t i = 0; i < hm->size; ++i) {
        slots[i] = hm->slots[i];
    }

    free(hm->slots);
    hm->slots = slots;
}

struct Hash_Map *make_hashmap()
{
    struct Hash_Map *hm = malloc(sizeof(struct Hash_Map));
    assert(hm != NULL);

    hm->size = 0;
    hm->cap = 4;

    hm->slots = calloc(hm->cap, sizeof(struct Slot));
    assert(hm->slots != NULL);

    return hm;
}

void free_hashmap(struct Hash_Map *hm)
{
    free(hm->slots);
    free(hm);
}

void hashmap_insert(struct Hash_Map *hm, const char *k, void *v)
{
    assert(hm->size <= hm->cap);

    if (hm->size == hm->cap) {
        hashmap_resize(hm);
    }

    uint32_t hash = djb2((uint8_t *) k);
    size_t index = hash % hm->cap;

    while (hm->slots[index].key != NULL) {
        index = (index + 1) % hm->cap;
    }

    hm->slots[index].key = (char *) k;
    hm->slots[index].value = v;

    hm->size++;
}

void *hashmap_get(struct Hash_Map *hm, const char *k)
{
    uint32_t hash = djb2((uint8_t *) k);
    size_t index = hash % hm->cap;

    // @FIXME: infinite loop if slot does not exist
    while (strcmp(hm->slots[index].key, k) != 0) {
        index = (index + 1) % hm->cap;
    }

    return hm->slots[index].value;
}

int main() {
    struct Hash_Map *hm = make_hashmap();

    hashmap_insert(hm, "Test1", "1");
    hashmap_insert(hm, "Test2", "2");
    hashmap_insert(hm, "Test3", "3");
    hashmap_insert(hm, "Test4", "4");
    hashmap_insert(hm, "Test5", "5");
    hashmap_insert(hm, "Test6", "6");
    hashmap_insert(hm, "Test7", "7");
    hashmap_insert(hm, "Test8", "8");
    hashmap_insert(hm, "Test9", "9");
    hashmap_insert(hm, "Test10", "10");
    hashmap_insert(hm, "Test11", "11");
    hashmap_insert(hm, "Test12", "12");
    hashmap_insert(hm, "Test13", "13");
    hashmap_insert(hm, "Test14", "14");
    hashmap_insert(hm, "Test15", "15");
    hashmap_insert(hm, "Test16", "16");
    hashmap_insert(hm, "Test17", "17");

    hashmap_debug(hm);

    free_hashmap(hm);

    return 0;
}
