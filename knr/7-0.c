#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    if (argc == 1) {
        printf("Provide file or files\n");
        exit(1);
    }

    while (*++argv) {
        FILE *f = fopen(*argv, "r");
        if (f == NULL) {
            printf("File `%s` does not exist\n", *argv);
            exit(1);
        }

        int c;
        while ((c = fgetc(f)) != EOF) {
            printf("%c", c);
        }

        fclose(f);
    }

    return 0;
}
