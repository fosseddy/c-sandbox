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

    for (int j = 0; s2[j] != '\0'; ++j) {
        for (int i = 0; s1[i] != '\0'; ++i) {
            if (s1[i] == s2[j]) {
                result = i;
                break;
            }
        }

        if (result > -1) {
            break;
        }
    }

    return result;
}
