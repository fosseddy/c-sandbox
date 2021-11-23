#include <stdio.h>
#include <string.h>

int strindex_right(char *, char *);

int main(void)
{
    printf("%i\n", strindex_right("hello, world", "orl"));
    return 0;
}

int strindex_right(char *s, char *t)
{
    for (int i = strlen(s) - 1; i >= 0; --i) {
        int j, k;
        for (j = i, k = 0; t[k] != '\0' && s[j] == t[k]; ++j, ++k);

        if (k > 0 && t[k] == '\0') {
            return i;
        }
    }

    return -1;
}
