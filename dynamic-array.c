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

int main(void)
{
    struct Dyn_Arr *arr = make_dyn_arr();

    printf("size: %lu\n", arr->size);
    printf("capacity: %lu\n", arr->cap);
    printf("values:\n");
    for (size_t i = 0; i < arr->size; ++i) {
        printf("  %i\n", arr->values[i]);
    }

    free_dyn_arr(arr);

    return 0;
}
