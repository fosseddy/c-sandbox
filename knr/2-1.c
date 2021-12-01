#include <stdio.h>
#include <assert.h>

char c_to_lower(const char);
void s_to_lower(char *, const char *);
void s_to_lower_mut(char *);
unsigned int a_to_ui(const char *);
unsigned int h_to_i(const char *);
unsigned int i_pow(int, int);
unsigned int s_len(const char *);

int main(void)
{
    printf("%i\n", h_to_i("0xff12ae"));
    return 0;
}

char c_to_lower(const char c)
{
    if (c >= 'A' && c <= 'Z') {
        return c + 'a' - 'A';
    } else {
        return c;
    }
}

void s_to_lower(char *d, const char *s)
{
    unsigned int i = 0;
    while ((d[i] = c_to_lower(s[i])) != '\0') {
        ++i;
    }
}

void s_to_lower_mut(char *s)
{
    for (unsigned int i = 0; s[i] != '\0'; ++i) {
        s[i] = c_to_lower(s[i]);
    }
}

unsigned int a_to_ui(const char *s)
{
    unsigned int n = 0;
    for (unsigned int i = 0; s[i] != '\0'; ++i) {
        if (s[i] >= '0' && s[i] <= '9') {
            n = n * 10 + s[i] - '0';
        }
    }

    return n;
}

#define BASE_16 16
#define BASE_16_ASCII_OFFSET ('a' - 10)
unsigned int h_to_i(const char *s)
{
    unsigned int n = 0;
    unsigned int j = s_len(s) - 2 - 1;
    for (unsigned int i = 2; s[i] != '\0'; ++i) {
        if (s[i] >= '0' && s[i] <= '9') {
            n += (s[i] - '0') * i_pow(BASE_16, j);
        } else if (s[i] >= 'A' && s[i] <= 'F' || s[i] >= 'a' && s[i] <= 'f') {
            n += c_to_lower(s[i] - BASE_16_ASCII_OFFSET) * i_pow(BASE_16, j);
        } else {
            assert(0 && "Not Hex");
        }

        --j;
        assert(j >= 0);
    }

    return n;
}

unsigned int i_pow(int num, int base)
{
    if (base == 0) {
        return 1;
    }

    unsigned int res = num;
    for (unsigned int i = 1; i < base; ++i) {
        res *= num;
    }

    return res;
}

unsigned int s_len(const char *s)
{
    unsigned int i = 0;
    while (s[i] != '\0') {
        ++i;
    }

    return i;
}
