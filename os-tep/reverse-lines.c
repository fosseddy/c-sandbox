#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int main(int argc, char **argv)
{
    assert(argc > 1);
    argv++;

    while (*argv) {
        size_t arr_cap = 100;
        size_t arr_size = 0;
        size_t line_cap = 80;

        FILE *f = fopen(*argv, "r");
        if (f == NULL) {
            fprintf(stderr, "could not open file %s\n", *argv);
            return 1;
        }

        char **arr = malloc(arr_cap * sizeof(char *));
        assert(arr != NULL);

        while(!feof(f)) {
            char *line = calloc(line_cap, sizeof(char));
            assert(line != NULL);
            size_t i = 0;
            int c;
            while ((c = getc(f)) != EOF && c != '\n') {
                if (i == line_cap - 1) {
                    line_cap *= 2;
                    char *new_line = calloc(line_cap, sizeof(char));
                    assert(new_line != NULL);
                    strcpy(new_line, line);
                    free(line);
                    line = new_line;
                }

                line[i++] = c;
            }

            if (arr_size == arr_cap) {
                arr_cap *= 2;
                arr = realloc(arr, arr_cap * sizeof(char *));
                assert(arr != NULL);
            }

            arr[arr_size++] = line;
        }

        for (int i = arr_size - 1; i >= 0; --i) {
            printf("%s\n", arr[i]);
        }

        for (size_t i = 0; i < arr_size; ++i) {
            free(arr[i]);
        }

        free(arr);
        fclose(f);
        argv++;
    }

    return 0;
}
