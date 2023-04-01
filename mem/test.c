#include <assert.h>
#include <stddef.h>

#include "mem.h"

struct intarr {
    int size;
    int cap;
    int data_size;
    int *buf;
};

static void test_init()
{
    struct intarr arr, arr2;

    meminit((struct mem *) &arr, sizeof(int), 0);
    meminit((struct mem *) &arr2, sizeof(int), 32);

    assert(arr.size == 0);
    assert(arr.cap == 8);
    assert(arr.buf != NULL);

    assert(arr2.size == 0);
    assert(arr2.cap == 32);
    assert(arr2.buf != NULL);
}

static void test_memgrow()
{
    struct intarr arr;

    meminit((struct mem *) &arr, sizeof(int), 0);

    for (int i = 0; i < 15; ++i) {
        memgrow((struct mem *) &arr);
        arr.buf[arr.size++] = i;
    }

    assert(arr.size == 15);
    assert(arr.cap == 16);
    for (int i = 0; i < arr.size; ++i) {
        assert(arr.buf[i] == i);
    }
}

static void test_memgrow_add()
{
    struct intarr arr;

    meminit((struct mem *) &arr, sizeof(int), 0);

    arr.size += 15;
    memgrow((struct mem *) &arr);

    assert(arr.size == 15);
    assert(arr.cap == 16);
}

static void test_memnext()
{
    struct intarr arr;

    meminit((struct mem *) &arr, sizeof(int), 0);

    for (int i = 0; i < 15; ++i) {
        int *val = memnext((struct mem *) &arr);
        *val = i;
    }

    assert(arr.size == 15);
    assert(arr.cap == 16);
    for (int i = 0; i < arr.size; ++i) {
        assert(arr.buf[i] == i);
    }
}

int main(void)
{
    test_init();
    test_memgrow();
    test_memgrow_add();
    test_memnext();

    return 0;
}
