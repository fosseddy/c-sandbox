#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

void trim_left(char *, const char *);
void trim_right(char *, const char *);
void trim(char *, const char *);

int main(void)
{
    char *x = "  hello_ptr       ";
    char y[] = "    hello_arr  ";
    char z[21] = {0};

    trim(z, x);
    assert(strcmp(z, "hello_ptr") == 0);
    assert(strcmp(x, "  hello_ptr       ") == 0);

    trim(z, y);
    assert(strcmp(z, "hello_arr") == 0);
    assert(strcmp(y, "    hello_arr  ") == 0);

    trim(y, y);
    assert(strcmp(y, "hello_arr") == 0);

    return 0;
}

void trim_left(char *d, const char *t)
{
    while (isspace(*t)) ++t;
    while ((*d++ = *t++));
}

void trim_right(char *d, const char *t)
{
    const char *t_end = t + strlen(t) - 1;
    while (isspace(*t_end)) --t_end;

    while (d <= t_end) *d++ = *t++;
    *d = '\0';
}

void trim(char *d, const char *t)
{
    trim_left(d, t);
    trim_right(d, d);
}
