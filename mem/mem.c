#include <stdlib.h>
#include <stdio.h>

#include "mem.h"

struct mem {
    size_t size;
    size_t cap;
    void *buf;
};

int meminit(void *ptr, size_t datasize, size_t cap)
{
    struct mem *mem = ptr;

    if (cap == 0) {
        cap = 8;
    }

    mem->size = 0;
    mem->cap = cap;

    mem->buf = malloc(mem->cap * datasize);
    if (mem->buf == NULL) {
        perror("meminit malloc");
        return 0;
    }

    return 1;
}

int memgrow(void *ptr, size_t datasize)
{
    struct mem *mem = ptr;

    if (mem->size == mem->cap) {
        void *newbuf;
        size_t newcap = mem->cap * 2;

        newbuf = realloc(mem->buf, newcap * datasize);
        if (newbuf == NULL) {
            perror("memgrow malloc");
            return 0;
        }

        mem->cap = newcap;
        mem->buf = newbuf;
    }

    return 1;
}

void memfree(void *ptr)
{
    struct mem *mem = ptr;
    free(mem->buf);
}
