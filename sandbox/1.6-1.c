#include <stdio.h>

#define ARR_CAP 20

#define INSIDE 1
#define END 0

int main(void)
{
    unsigned int w_lens[ARR_CAP] = {0};
    unsigned char w_state = END;
    unsigned int w_len = 0;

    char c;
    while ((c = getchar()) != EOF) {
        if (c == ' ' || c == '\n' || c == '\t') {
            if (w_state == INSIDE) {
                ++w_lens[w_len];
                w_len = 0;
            }
            w_state = END;
        } else if (w_state == END) {
            w_state = INSIDE;
        }

        if (w_state == INSIDE) {
            ++w_len;
        }
    }

    unsigned int max = 0;
    unsigned int min = 420;

    for (unsigned int i = 0; i < ARR_CAP; ++i) {
        if (w_lens[i] == 0) {
            continue;
        }

        if (w_lens[i] > max) {
            max = w_lens[i];
        }

        if (w_lens[i] < min) {
            min = w_lens[i];
        }
    }


    printf("    ");
    for (unsigned int i = min; i <= max; ++i) {
        printf("%3i ", i);
    }
    printf("\n");

    for (unsigned int i = 0; i < ARR_CAP; ++i) {
        if (w_lens[i] == 0) {
            continue;
        }
        printf("%3i ", i);
        for (unsigned int j = 0; j < w_lens[i]; ++j) {
            printf("%3c ", '*');
        }
        printf("\n");
    }

    return 0;
}
