#include <stdlib.h>
#include <stdio.h>

#include "mem.h"

int meminit(struct mem *mem, size_t datasize, size_t cap)
{
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

int memgrow(struct mem *mem, size_t datasize)
{
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
