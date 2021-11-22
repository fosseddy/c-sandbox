#include <stdio.h>

int any(char *, char *);

int main(void)
{
    char s1[11] = "1a3c56d8b0";
    char s2[3] = "8b";

    printf("%i\n", any(s1, s2));

    return 0;
}

int any(char *s1, char *s2)
{
    int result = -1;

    for (int i = 0; s2[i] != '\0'; ++i) {
        for (int j = 0; s1[j] != '\0'; ++j) {
            if (s1[j] == s2[i]) {
                result = j;
                break;
            }
        }

        if (result > -1) {
            break;
        }
    }

    return result;
}
