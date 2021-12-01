#include <stdio.h>
#include <stdlib.h>

#define LINE_CAP 1000

int readline(char *, FILE *);

int main(int argc, char **argv)
{
    if (argc < 3) {
        printf("Provide 2 files\n");
        exit(1);
    }

    FILE *first = fopen(*++argv, "r");
    if (first == NULL) {
        printf("Could not open file: %s\n", *argv);
        exit(1);
    }

    FILE *second = fopen(*++argv, "r");
    if (second == NULL) {
        printf("Could not open file: %s\n", *argv);
        exit(1);
    }

    char line_f[LINE_CAP] = {0};
    char line_s[LINE_CAP] = {0};

    while (1) {
        int read_f = readline(line_f, first);
        int read_s = readline(line_s, second);

        if (read_f != read_s) {
            printf("diff:\n");
            printf("%s\n", line_f);
            printf("%s\n", line_s);
            exit(0);
        }

        if (read_f == -1 || read_s == -1) {
            break;
        }

        int i = 0;
        for (; i < read_f && line_f[i] == line_s[i]; ++i);
        if (i < read_f) {
            printf("diff:\n");
            printf("%s\n", line_f);
            printf("%s\n", line_s);
            exit(0);
        }
    }

    printf("Files are equal\n");

    return 0;
}

int readline(char *line, FILE *f)
{
    int c;
    int read = 0;
    while ((c = fgetc(f)) != '\n' && c != EOF) {
        *line = c;
        ++line;
        ++read;
    }
    *line = '\0';

    if (c == EOF) {
        read = -1;
    }

    return read;
}
