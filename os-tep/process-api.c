#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

void change_var(void)
{
    int x = 100;

    pid_t child_pid = fork();

    if (child_pid < 0) {
        fprintf(stderr, "could not create child process\n");
        exit(1);
    } else if (child_pid == 0) {
        x += 13;
    } else {
        x += 8;
    }

    printf("pid: %i, x: %i\n", child_pid, x);
}

int main(void)
{
    change_var();

    return 0;
}

