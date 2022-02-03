#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct Dynarr {
    size_t size, cap;
    void *values;
};

#define START_CAP 8

#define make_dynarr(arr, type)                          \
    do {                                                \
        arr = malloc(sizeof(struct Dynarr));            \
        assert(arr != NULL);                            \
        arr->size = 0;                                  \
        arr->cap = START_CAP;                           \
        type *values = malloc(arr->cap * sizeof(type)); \
        assert(values != NULL);                         \
        arr->values = values;                           \
    } while (0);                                        \

#define dynarr_put(arr, type, v)                                         \
    do {                                                                 \
        assert(arr->size <= arr->cap);                                   \
        if (arr->size == arr->cap) {                                     \
            arr->cap *= 2;                                               \
            arr->values = realloc(arr->values, arr->cap * sizeof(type)); \
            assert(arr->values != NULL);                                 \
        }                                                                \
        ((type *) arr->values)[arr->size++] = v;                         \
    } while (0);                                                         \

#define dynarr_get(arr, type, i, val)    \
    do {                                 \
        assert(i < dynarr_size(arr));    \
        val = ((type *) arr->values)[i]; \
    } while (0);                         \

void free_dynarr(struct Dynarr *arr)
{
    free(arr->values);
    free(arr);
}

size_t dynarr_size(struct Dynarr *arr)
{
    return arr->size;
}

struct Point {
    int x, y;
};

void dynarr_putpoint(struct Dynarr *arr, struct Point v)
{
    dynarr_put(arr, struct Point, v);
}

struct Point dynarr_getpoint(struct Dynarr *arr, size_t i)
{
    struct Point p;
    dynarr_get(arr, struct Point, i, p);
    return p;
}

void dynarr_puts(struct Dynarr *arr, char *v)
{
    dynarr_put(arr, char *, v);
}

char *dynarr_gets(struct Dynarr *arr, size_t i)
{
    char *s;
    dynarr_get(arr, char *, i, s);
    return s;
}

void dynarr_puti(struct Dynarr *arr, int v)
{
    dynarr_put(arr, int, v);
}

int dynarr_geti(struct Dynarr *arr, size_t i)
{
    int v;
    dynarr_get(arr, int, i, v);
    return v;
}

int main(void)
{
    struct Dynarr *parr = NULL;
    make_dynarr(parr, struct Point);

    for (size_t i = 0; i < 31; ++i) {
        struct Point p = {i, i};
        dynarr_putpoint(parr, p);
    }

    printf("struct Point array\n");
    printf("size: %lu\n", parr->size);
    printf("capacity: %lu\n", parr->cap);
    printf("values:\n");
    for (size_t i = 0; i < dynarr_size(parr); ++i) {
        struct Point val = dynarr_getpoint(parr, i);
        printf("    %d", val.x);
        printf(" %d\n", val.y);
    }

    printf("\n");

    struct Dynarr *sarr = NULL;
    make_dynarr(sarr, char *);

    dynarr_puts(sarr, "hello");
    dynarr_puts(sarr, "world");
    dynarr_puts(sarr, "how");
    dynarr_puts(sarr, "are");
    dynarr_puts(sarr, "you");
    dynarr_puts(sarr, "doing");

    printf("string array\n");
    printf("size: %lu\n", sarr->size);
    printf("capacity: %lu\n", sarr->cap);
    printf("values:\n");
    for (size_t i = 0; i < dynarr_size(sarr); ++i) {
        printf("    %s\n", dynarr_gets(sarr, i));
    }

    printf("\n");

    printf("integer array\n");
    struct Dynarr *iarr = NULL;
    make_dynarr(iarr, int);

    for (int i = 0; i < 15; ++i) {
        dynarr_puti(iarr, i);
    }

    printf("size: %lu\n", iarr->size);
    printf("capacity: %lu\n", iarr->cap);
    printf("values:\n");
    for (size_t i = 0; i < dynarr_size(iarr); ++i) {
        printf("    %d\n", dynarr_geti(iarr, i));
    }

    free_dynarr(parr);
    free_dynarr(sarr);
    free_dynarr(iarr);
    return 0;
}
