#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/wait.h>
#include <assert.h>

#define INPUT_CAP 1000

struct Cmd {
    char *path;
    char **args;

    size_t args_size;
};

void cmd_debug(struct Cmd *cmd)
{
    printf("cmd->path: %s\n", cmd->path);
    printf("cmd->args_size: %lu\n", cmd->args_size);
    printf("cmd->args:\n");
    for (size_t i = 0; i < cmd->args_size; ++i) {
        printf("    %lu: %s\n", i, cmd->args[i]);
    }
}

void read_input(char *buf, size_t n)
{
    size_t i = 0;
    int c;

    while ((c = getchar()) != EOF && c != '\n') {
        assert(i < n - 1);
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

    for (;;) {
        printf("wish> ");

        char input[INPUT_CAP] = {0};
        read_input(input, INPUT_CAP);

        struct Cmd *cmd = malloc(sizeof(struct Cmd));
        assert(cmd != NULL);

        cmd->path = calloc(50, sizeof(char));
        assert(cmd->path != NULL);

        cmd->args = calloc(10, sizeof(char *));
        assert(cmd->args != NULL);

        cmd->args_size = 0;

        for (size_t i = 0; input[i] != '\0'; ++i) {
            char *buf = calloc(50, sizeof(char));
            size_t j = 0;
            while (input[i] != ' ' && input[i] != '\0') {
                buf[j++] = input[i++];
            }

            if (cmd->args_size == 0) {
                char tt[50] = {0};
                strcat(tt, "/bin/");
                strcat(tt, buf);
                strcpy(cmd->path, tt);
            }

            cmd->args[cmd->args_size++] = buf;
        }

        if (strcmp(cmd->args[0], "exit") == 0) {
            free(cmd->path);
            for (size_t i = 0; i < cmd->args_size; ++i) {
                free(cmd->args[i]);
            }
            free(cmd->args);
            free(cmd);

            return 0;
        }

        pid_t cid = fork();
        int exec_success = 0;
        if (cid < 0) {
            fprintf(stderr, "could not create child process\n");
            return 1;
        } else if (cid == 0) {
            exec_success = execv(cmd->path, cmd->args);
            printf("wish: %s: command not found\n", cmd->args[0]);
        } else {
            waitpid(cid, NULL, 0);
        }

        free(cmd->path);
        for (size_t i = 0; i < cmd->args_size; ++i) {
            free(cmd->args[i]);
        }
        free(cmd->args);
        free(cmd);

        if (exec_success == -1) return 1;
    }

    return 0;
}
