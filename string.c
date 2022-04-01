#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

size_t strsplitc(char ***buf, char *s, char d)
{
    assert(*buf == NULL);

    size_t cap = 4;
    size_t len = 0;

    *buf = malloc(cap * sizeof(char *));
    assert(*buf != NULL);

    size_t begin = 0;
    size_t i = 0;
    for (i = 0; s[i] != '\0'; ++i) {
        if (begin > i) {
            begin = i + 1;
        } else if (s[i] == d && begin < i) {
            char *word = calloc(i - begin + 1, sizeof(char));
            assert(word != NULL);

            for (size_t j = begin, k = 0; j < i; ++j, ++k) {
                word[k] = s[j];
            }

            (*buf)[len++] = word;

            begin = i + 1;
        }
    }

    if (begin < i) {
        char *word = calloc(i - begin + 1, sizeof(char));
        assert(word != NULL);

        for (size_t j = begin, k = 0; j < i; ++j, ++k) {
            word[k] = s[j];
        }

        (*buf)[len++] = word;
    }

    return len;
}

int main(void)
{
    char **arr = NULL;

    //size_t arr_len = strsplitc(&arr, "hello, world", ',');
    // ['hello', ' world'

    //size_t arr_len = strsplitc(&arr, "hello, world", 'o');
    // ['hell', ', w', 'rld']

    //size_t arr_len = strsplitc(&arr, "hello, world", 'd');
    // ['hello, worl']

    //size_t arr_len = strsplitc(&arr, "hello, world", 'c');
    // ['hello, world']

    size_t arr_len = strsplitc(&arr, "hello, world", 'l');
    // ['he', 'o, wor', 'd']

    for (size_t i = 0; i < arr_len; ++i) {
        printf("%s\n", arr[i]);
    }

    return 0;
}
