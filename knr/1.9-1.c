#include <stdio.h>

int readline(char *dest, int lim);
void copy(char *dest, char *v);
int s_len(char *s);

#define BUFF_CAP 1000
#define LINES_CAP 10
#define PRINTABLE_LEN 10

int main(void)
{
    char lines[LINES_CAP][BUFF_CAP] = {0};
    char buff[BUFF_CAP] = {0};

    int i = 0;
    int len = 0;
    while ((len = readline(buff, BUFF_CAP)) > 0) {
        if (len >= PRINTABLE_LEN && i < LINES_CAP && len < BUFF_CAP) {
            copy(lines[i], buff);
            ++i;
        }
    }

    for (int i = 0; i < LINES_CAP; ++i) {
        if (s_len(lines[i]) > 0) {
            printf("%s\n", lines[i]);
        }
    }

    return 0;
}

int readline(char *dest, int lim)
{
    int i = 0;
    for (; i < lim - 1; ++i) {
        char c = getchar();

        if (c == EOF || c == '\n' || c == '0') {
            break;
        }

        dest[i] = c;
    }

    dest[i] = '\0';

    return i;
}

void copy(char *dest, char *v)
{
    int i = 0;
    while ((dest[i] = v[i]) != '\0') {
        ++i;
    }
}

int s_len(char *s)
{
    int len = 0;
    while (s[len] != '\0') {
        ++len;
    }

    return len;
}
