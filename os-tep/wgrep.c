#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

int main(int argc, char **argv)
{
    if (argc < 3) {
        return 0;
    }

    argv++;

    char *word = *argv;
    argv++;

    while (*argv) {
        FILE *f = fopen(*argv, "r");
        if (f == NULL) {
            fprintf(stderr, "could not open file %s\n", *argv);
            return 1;
        }

        while (!feof(f)) {
            char buf[1000] = {0};
            size_t i = 0;
            int c;
            while ((c = getc(f)) != EOF && c != '\n') {
                assert(i < 1000);
                buf[i++] = c;
            }

            for (size_t i = 0; i < strlen(buf); ++i) {
                if (word[0] != buf[i]) continue;

                int match = 1;
                for (size_t j = 1, k = i + 1; j < strlen(word); ++j, ++k) {
                    if (word[j] != buf[k]) {
                        match = 0;
                        break;
                    }
                }

                if (match) {
                    printf("%s\n", buf);
                    break;
                }
            }

        }

        fclose(f);
        argv++;
    }

    return 0;
}
