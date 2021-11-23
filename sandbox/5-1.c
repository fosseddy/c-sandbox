#include <stdio.h>
#include <string.h>
#include <assert.h>

void s_cat(char *, const char *);
unsigned int s_end(const char *, const char *);
void s_reverse(char *);
int s_index(const char *, const char *)

int main(void)
{
    char s[15] = "hello";

    s_cat(s, ", world");
    assert(strcmp(s, "hello, world") == 0);

    s[0] = '\0';

    s_cat(s, "this is test");
    assert(strcmp(s, "this is test") == 0);


    assert(s_end("hello, world", "rld") == 1);
    assert(s_end("hello, world", ", world") == 1);
    assert(s_end("hello, world", "rd") == 0);

    char x[10] = "hello";
    s_reverse(x);
    assert(strcmp(x, "olleh") == 0);

    return 0;
}

void s_cat(char *d, const char *t)
{
    while (*d) ++d;
    while ((*d++ = *t++));
}

unsigned int s_end(const char *s, const char *t)
{
    int offset = strlen(s) - strlen(t);
    s = s + offset;

    while (*s++ == *t++) {
        if (*s == '\0') {
            return 1;
        }
    }

    return 0;
}

void s_reverse(char *s)
{
    unsigned int offset = strlen(s) - 1;
    char *s_end = s + offset;

    while (s < s_end) {
        char tmp = *s;
        *s++ = *s_end;
        *s_end-- = tmp;
    }
}

int s_index(const char *s, const char *t)
{
}
