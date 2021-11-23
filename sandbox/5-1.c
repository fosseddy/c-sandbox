#include <stdio.h>
#include <string.h>
#include <assert.h>

void s_cat(char *, const char *);

int main(void)
{
    char s[20] = "hello";
    s_cat(s, ", world");

    assert(strcmp(s, "hello, world") == 0);


    return 0;
}

void s_cat(char *d, const char *t)
{
    while (*d) ++d;
    while ((*d++ = *t++));
}
