#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "array.h"

int array_init(struct array *arr, size_t membsize, size_t cap)
{
    if (cap == 0) {
        cap = 8;
    }

    arr->cap = cap;
    arr->size = 0;
    arr->membsize = membsize;

    arr->buf = malloc(arr->cap * arr->membsize);
    if (arr->buf == NULL) {
        return 0;
    }

    return 1;
}

int array_add(struct array *arr, void *value)
{
    uint8_t *buf;

    if (arr->size == arr->cap) {
        void *newptr;
        size_t newcap = arr->cap * 2;

        newptr = realloc(arr->buf, newcap * arr->membsize);
        if (newptr == NULL) {
            return 0;
        }

        arr->cap = newcap;
        arr->buf = newptr;
    }

    buf = arr->buf;
    memcpy(buf + arr->size * arr->membsize, value, arr->membsize);
    arr->size++;

    return 1;
}

void *array_at(struct array *arr, size_t index)
{
    if (index >= arr->size) {
        return NULL;
    }

    return (uint8_t*) arr->buf + index * arr->membsize;
}

void array_free(struct array *arr)
{
    free(arr->buf);
}
