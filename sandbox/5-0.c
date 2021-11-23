#include <stdio.h>
#include <ctype.h>
#include <string.h>

void trim_left(char *, const char *);
void trim_right(char *, const char *);
void trim(char *, const char *);

int main(void)
{
    char *x = "  hello_ptr       ";
    char y[] = "    hello_arr  ";
    char z[21] = {0};

    printf("x: #%s#\n", x);
    trim(z, x);
    printf("x: #%s#\n", x);
    printf("z: #%s#\n", z);

    printf("\n");

    printf("y: #%s#\n", y);
    trim(z, y);
    printf("y: #%s#\n", y);
    printf("z: #%s#\n", z);

    printf("\n");

    printf("y: #%s#\n", y);
    trim(y, y);
    printf("y: #%s#\n", y);
    printf("y: #%s#\n", y);

    return 0;
}

void trim_left(char *d, const char *t)
{
    while (isspace(*t)) ++t;
    while ((*d++ = *t++));
}

void trim_right(char *d, const char *t)
{
    int offset = strlen(t) - 1;
    for (; isspace(t[offset]); --offset);

    for (int i = 0; i <= offset; ++i) *d++ = *t++;
    *d = '\0';
}

void trim(char *d, const char *t)
{
    trim_left(d, t);
    trim_right(d, d);
}
