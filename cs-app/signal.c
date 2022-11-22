#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <assert.h>

static char *msg;

void signal_handler(int sig)
{
    printf("%s\n", msg);
}

int main(int argc, char **argv)
{
    argv++;

    size_t len = 0;
    size_t cap = 10;

    msg = malloc(cap * sizeof(char));
    assert(msg);

    while (*argv) {
        char *s = *argv;
        while (*s != '\0') {
            if (len >= cap) {
                cap *= 2;
                msg = realloc(msg, cap * sizeof(char));
            }
            msg[len++] = *s++;
        }

        argv++;

        if (*argv) {
            if (len >= cap) {
                cap *= 2;
                msg = realloc(msg, cap * sizeof(char));
            }
            msg[len++] = ' ';
        }
    }

    if (len >= cap) {
        cap *= 2;
        msg = realloc(msg, cap * sizeof(char));
    }
    msg[len++] = '\0';

    struct sigaction sa;
    sa.sa_handler = signal_handler;
    sigaction(SIGALRM, &sa, NULL);

    alarm(3);
    pause();

    return 0;
}
