#include <stdio.h>

int readline(char *dest, int lim);
void copy(char *dest, char *v);

#define BUFF_CAP 1000

int main(void)
{
    int len = 0;
    char buff[BUFF_CAP] = {0};

    char longest[BUFF_CAP] = {0};
    int longest_len = 0;

    while ((len = readline(buff, BUFF_CAP)) > 0) {
        if (len > longest_len) {
            copy(longest, buff);
            longest_len = len;
        }
    }

    if (longest_len > 0) {
        printf("%i -- %s\n", longest_len, longest);
    }

    return 0;
}

int readline(char *dest, int lim)
{
    int i = 0;
    for (; i < lim - 1; ++i) {
        char c = getchar();

        if (c == EOF || c == '\n') {
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
