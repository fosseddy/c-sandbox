#include <stdlib.h>
#include <stdio.h>

#include "mem.h"

enum { INIT_CAP = 8 };

int meminit(struct mem *m, unsigned long data_size, unsigned long cap)
{
    if (cap == 0) {
        cap = INIT_CAP;
    }

    m->size = 0;
    m->cap = cap;
    m->data_size = data_size;

    m->buf = malloc(m->cap * m->data_size);
    if (m->buf == NULL) {
        perror("meminit malloc");
        return 0;
    }

    return 1;
}

int memgrow(struct mem *m)
{
    if (m->size >= m->cap) {
        void *newbuf;
        unsigned long newcap = m->cap * 2;

        while (newcap < m->size) {
            newcap *= 2;
        }

        newbuf = realloc(m->buf, newcap * m->data_size);
        if (newbuf == NULL) {
            perror("memgrow malloc");
            return 0;
        }

        m->cap = newcap;
        m->buf = newbuf;
    }

    return 1;
}

void memfree(struct mem *m)
{
    free(m->buf);
}
