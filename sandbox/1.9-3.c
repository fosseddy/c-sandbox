#include <stdio.h>

int readline(char *dest, int lim);
int s_len(char *s);
void reverse(char *s);

#define BUFF_CAP 1000

int main(void)
{
    char buff[BUFF_CAP] = {0};

    int len = 0;
    while ((len = readline(buff, BUFF_CAP)) > 0) {
        if (len < BUFF_CAP) {
            reverse(buff);
            printf("%s\n", buff);
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

int s_len(char *s)
{
    int len = 0;
    while (s[len] != '\0') {
        ++len;
    }

    return len;
}

void reverse(char *s)
{
    int len = s_len(s);
    if (len == 0) return;

    int i = 0;
    int j = len - 1;
    while (i < len / 2) {
        char tmp = s[i];
        s[i++] = s[j];
        s[j--] = tmp;
    }
}
