#include <stdio.h>
#include <ctype.h>
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

        int c;
        while ((c = getc(f)) != EOF) {
            int count = 1;
            int cc;
            while ((cc = getc(f)) == c) {
                count++;
            }

            printf("%i%c", count, c);
            if (!isspace(cc)) {
                ungetc(cc, f);
            }
        }

        fclose(f);
        argv++;
    }

    return 0;
}
