#include <stdio.h>
#include <assert.h>

int s_len(char *s);
int is_space(char c);
int readline(char *d, int lim);

#define BUF_CAP 256
#define LINES_CAP 10
#define FOLD_COL 20

int main(void)
{
    while (1) {
        char lines[LINES_CAP][BUF_CAP] = {0};

        // read line
        int line_count = 0;
        int len_read = readline(lines[line_count], BUF_CAP);
        assert(len_read < BUF_CAP);
        if (len_read == 0) {
            break;
        }

        // fold line if needed
        for (int i = 0; s_len(lines[line_count + i]) > FOLD_COL; ++i) {
            assert(line_count + i < LINES_CAP);

            int fold_i = FOLD_COL - 1;
            while (!is_space(lines[line_count + i][fold_i])) {
                --fold_i;
            }

            lines[line_count + i][fold_i++] = '\0';

            int j = 0;
            int next_line_i = line_count + i + 1;
            assert(next_line_i < LINES_CAP);
            while (lines[line_count + i][fold_i] != '\0') {
                lines[next_line_i][j++] = lines[line_count + i][fold_i++];
            }

            lines[next_line_i][j] = '\0';
        }

        for (int i = 0; i < LINES_CAP; ++i) {
            if (s_len(lines[i]) > 0) {
                printf("%s\n", lines[i]);
            }
        }

        printf("\n");
    }

    return 0;
}

int s_len(char *s)
{
    int len = 0;
    while (s[len] != '\0') {
        ++len;
    }

    return len;
}

int is_space(char c)
{
    return c == ' ' || c == '\t' || c == '\n';
}

int readline(char *d, int lim)
{
    int i = 0;
    for (; i < lim - 1; ++i) {
        char c = getchar();

        if (c == EOF || c == '\n') {
            break;
        }

        d[i] = c;
    }

    d[i] = '\0';

    return i;
}
