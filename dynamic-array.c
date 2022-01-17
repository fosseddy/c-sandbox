#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

struct Dyn_Arr {
    size_t size;
    size_t cap;
    int *values;
};

#define START_CAP 8
struct Dyn_Arr *make_dyn_arr(void)
{
    struct Dyn_Arr *arr = malloc(sizeof(struct Dyn_Arr));
    assert(arr != NULL);

    arr->size = 0;
    arr->cap = START_CAP;

    int *values = malloc(arr->cap * sizeof(int));
    assert(values != NULL);

    arr->values = values;

    return arr;
}

void free_dyn_arr(struct Dyn_Arr *arr)
{
    free(arr->values);
    free(arr);
}

void dyn_arr_debug(struct Dyn_Arr *arr)
{
    printf("size: %lu\n", arr->size);
    printf("capacity: %lu\n", arr->cap);
    printf("values:\n");
    for (size_t i = 0; i < arr->size; ++i) {
        printf("  %i\n", arr->values[i]);
    }
}

void dyn_arr_put(struct Dyn_Arr *arr, int value)
{
    assert(arr->size <= arr->cap);

    if (arr->size == arr->cap) {
        arr->cap *= 2;

        int *values = malloc(arr->cap * sizeof(int));
        assert(values != NULL);

        for (size_t i = 0; i < arr->size; ++i) {
            values[i] = arr->values[i];
        }

        free(arr->values);
        arr->values = values;
    }

    arr->values[arr->size++] = value;
}

int main(void)
{
    struct Dyn_Arr *arr = make_dyn_arr();
    dyn_arr_debug(arr);

    dyn_arr_put(arr, 0);
    dyn_arr_put(arr, 1);
    dyn_arr_put(arr, 2);
    dyn_arr_put(arr, 3);
    dyn_arr_put(arr, 4);
    dyn_arr_put(arr, 5);
    dyn_arr_put(arr, 6);
    dyn_arr_put(arr, 7);
    dyn_arr_put(arr, 8);

    dyn_arr_debug(arr);

    free_dyn_arr(arr);
    return 0;
}
