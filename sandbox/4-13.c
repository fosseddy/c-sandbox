#include <stdio.h>
#include <string.h>

void reverse_rec(char *, int, int);

int main(void)
{
    char x[] = "hello, world";
    printf("%s\n", x);

    reverse_rec(x, 0, strlen(x) - 1);

    printf("%s\n", x);

    return 0;
}

void reverse_rec(char *s, int i, int j)
{
    if (i < j) {
        char tmp = s[i];
        s[i++] = s[j];
        s[j--] = tmp;

        reverse_rec(s, i, j);
    }
}
