#include <stdio.h>

int main(void)
{
    unsigned int wsc = 0;
    unsigned long int cc = 0;
    unsigned int digits[10] = {0};

    int c;
    while ((c = getchar()) != EOF) {
        if (c == ' ' || c == '\n' || c == '\t') {
            ++wsc;
        } else if (c >= '0' && c <= '9') {
            ++digits[c - '0'];
        } else {
            ++cc;
        }
    }

    printf("digits:\n");
    for (unsigned int i = 0; i < 10; ++i) {
        printf("  %i -> %i\n", i, digits[i]);
    }

    printf("white space: %i\n", wsc);
    printf("other chars: %li\n", cc);

    return 0;
}
