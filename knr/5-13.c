#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define N 10

int main(int argc, char **argv)
{
    (void) argc;

    int n = N;

    while (*++argv) {
        char *arg = *argv;

        if (arg[0] == '-') {
            ++arg;
            n = atoi(arg);
        }
    }

    if (n <= 0) {
        n = N;
    }

    char *lines[50] = {0};
    int i = 0;
    int c = '\0';
    for (;;) {
        c = getchar();

        if (c == EOF) {
            break;
        }

        char line[1000] = {0};
        int j = 0;

        while (c != '\n' && c != EOF) {
            line[j++] = c;
            c = getchar();
        }
        line[j] = '\0';

        char *p = malloc(sizeof(char) * strlen(line) + 1);
        strcpy(p, line);
        lines[i++] = p;
    }

    int num = (n > i) ? 0 : i - n;
    for (int k = i - 1; k >= num; --k) {
        printf("%s\n", lines[k]);
    }

    return 0;
}
