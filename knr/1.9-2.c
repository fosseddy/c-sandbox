#include <stdio.h>

int readline(char *dest, int lim);
void trim(char *dest, char *s);
int s_len(char *s);

#define BUFF_CAP 1000

int main(void)
{
    char buff[BUFF_CAP] = {0};
    char line[BUFF_CAP] = {0};

    int len = 0;
    while ((len = readline(buff, BUFF_CAP)) > 0) {
        if (len < BUFF_CAP) {
            trim(line, buff);
            if (s_len(line) > 0) {
                printf("%s\n", line);
            }
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

void trim(char *dest, char *s)
{
    // trim left
    int left_pad = 0;
    while (s[left_pad] == ' ' || s[left_pad] == '\t') {
        ++left_pad;
    }

    // trim right
    int right_pad = s_len(s);
    if (right_pad > left_pad) {
        --right_pad;
    }
    while (s[right_pad] == ' ' || s[right_pad] == '\t') {
        --right_pad;
    }

    int i = 0;
    while (left_pad <= right_pad) {
        dest[i++] = s[left_pad++];
    }

    dest[i] = '\0';
}
