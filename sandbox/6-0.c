#include <stdio.h>

struct key {
    char *value;
    size_t count;
};

static struct key keys[] = {
    { "one", 0 },
    { "two", 0 },
    { "three", 0 },
    { "four", 0 }
};

#define N (sizeof(keys) / sizeof(keys[0]))

int main(void)
{
    struct key *begin = &keys[0];
    struct key *end = &keys[N];

    printf("b: %p\ne: %p\n", begin, end);

    printf("num of elems: %i\n", end - begin);
    printf("mid: %i\n", (end - begin) / 2);
    printf("mid address: %p\n", begin + (end - begin) / 2);

    for (size_t i = 0; i < N; ++i) {
        printf("%s -- %li\n", keys[i].value, keys[i].count);
    }

    return 0;
}
