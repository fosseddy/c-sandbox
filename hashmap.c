#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
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
    int value;
};

struct Hash_Map {
    size_t size;
    size_t cap;
    struct Slot *slots;
};

static void hashmap_resize(struct Hash_Map *hm)
{
    assert(hm->cap != 0);
    assert(hm->size == hm->cap);

    hm->cap *= 2;
    struct Slot *new_slots = calloc(hm->cap, sizeof(struct Slot));
    assert(new_slots != NULL);

    for (size_t i = 0; i < hm->size; ++i) {
        new_slots[i] = hm->slots[i];
    }

    free(hm->slots);
    hm->slots = new_slots;
}

#define HASH_MAP_DEFAULT_CAP 4
struct Hash_Map *make_hashmap()
{
    struct Hash_Map *hm = malloc(sizeof(struct Hash_Map));
    assert(hm != NULL);

    hm->size = 0;
    hm->cap = HASH_MAP_DEFAULT_CAP;

    struct Slot *slots = calloc(hm->cap, sizeof(struct Slot));
    assert(slots != NULL);

    hm->slots = slots;

    return hm;
}

void free_hashmap(struct Hash_Map *hm)
{
    assert(hm != NULL);
    assert(hm->slots != NULL);

    free(hm->slots);
    free(hm);
}

void hashmap_put(struct Hash_Map *hm, char *k, int v)
{
    assert(hm->size <= hm->cap);

    if (hm->size == hm->cap) {
        hashmap_resize(hm);
    }

    uint32_t index = djb2((uint8_t *) k) % hm->cap;
    hm->slots[index].key = k;
    hm->slots[index].value = v;
    hm->size++;
}

int main() {
    struct Hash_Map *hm = make_hashmap();

    hashmap_put(hm, "zero", 0);
    hashmap_put(hm, "one", 1);
    hashmap_put(hm, "two", 2);
    hashmap_put(hm, "three", 3);
    hashmap_put(hm, "four", 4);
    hashmap_put(hm, "five", 5);

    for (size_t i = 0; i < hm->cap; ++i) {
        printf("%3li -> key: %10s; value: %3i;\n", i, hm->slots[i].key, hm->slots[i].value);
    }

    free_hashmap(hm);

    return 0;
}
