#include <stdio.h>
#include <string.h>
#include <assert.h>

#define INPUT_CAP 1000

void read_input(char *buf, size_t n)
{
    size_t i = 0;
    int c;

    while ((c = getchar()) != EOF && c != '\n' && i < n - 1) {
        buf[i++] = c;
    }
}

int main(int argc, char **argv)
{
    if (argc > 2) {
        fprintf(stderr, "Usage: wish [command | batch.txt]\n");
        return 1;
    }

    if (argc == 2) {
        argv++;
        printf("batch file is: %s\n", *argv);
        return 0;
    }

    int exit = 0;
    while (!exit) {
        printf("wish> ");
        char input[INPUT_CAP] = {0};
        read_input(input, INPUT_CAP);

        if (strcmp(input, "exit") == 0) {
            exit = 1;
        }
    }

    return 0;
}
