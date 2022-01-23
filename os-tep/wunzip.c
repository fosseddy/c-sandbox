#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char **argv)
{
    assert(argc > 1);
    argv++;

    while (*argv) {
        FILE *f = fopen(*argv, "r");
        if (f == NULL) {
            fprintf(stderr, "could not open file %s\n", *argv);
            return 1;
        }

        for (;;) {
            char buf[10000] = {0};
            size_t i = 0;
            int c;
            while ((c = getc(f)) != EOF && isdigit(c)) {
                buf[i++] = c;
            }

            if (c == EOF) break;

            size_t len = atoi(buf);
            for (size_t i = 0; i < len; ++i) {
                printf("%c", c);
            }
        }

        printf("\n");
        fclose(f);
        argv++;
    }

    return 0;
}
