#include <stdio.h>

void squeeze(char *, char *);

int main(void)
{
    char s1[11] = "1a3c56d8b0";
    char s2[7] = "1a3c8b";

    squeeze(s1, s2);

    printf("%s\n", s1);

    return 0;
}

void squeeze(char *s1, char *s2)
{
    int k = 0;
    int is_match = 0;

    for (int i = 0; s1[i] != '\0'; ++i) {
        for (int j = 0; s2[j] != '\0'; ++j) {
            if (s1[i] == s2[j]) {
                is_match = 1;
                break;
            }

            is_match = 0;
        }

        if (!is_match) {
            s1[k++] = s1[i];
        }
    }

    s1[k] = '\0';
}
