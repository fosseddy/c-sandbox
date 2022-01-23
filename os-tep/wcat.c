#include <stdio.h>

int main(int argc, char **argv)
{
    if (argc < 2) {
        return 0;
    }

    argv++;

    while (*argv) {
        FILE *f = fopen(*argv, "r");
        if (f == NULL) {
            fprintf(stderr, "could not open file %s\n", *argv);
            return 1;
        }

        int c;
        while ((c = getc(f)) != EOF) {
            fprintf(stdout, "%c", c);
        }

        fclose(f);
        argv++;
    }

    return 0;
}
