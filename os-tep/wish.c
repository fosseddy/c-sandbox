#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>
#include <assert.h>

#define INPUT_CAP 1000

#define ARGS_DEFAULT_CAP 8

struct Cmd {
    char *name;

    char **args;
    size_t args_cap;
    size_t args_size;
};

struct Shell {
    char paths[5][100];
    size_t paths_size;

    int exit;
};

int main(void)
{
    struct Shell *shell = malloc(sizeof(struct Shell));
    assert(shell != NULL);

    shell->exit = 0;
    shell->paths_size = 0;

    strcpy(shell->paths[shell->paths_size++], "/bin");

    while (!shell->exit) {
        char input[INPUT_CAP] = {0};

        {
            size_t i = 0;
            int c;
            while ((c = getchar()) != EOF && c != '\n') {
                assert(i < INPUT_CAP - 1);
                input[i++] = c;
            }
        }

        if (strlen(input) == 0) continue;

        struct Cmd *cmd = malloc(sizeof(struct Cmd));
        assert(cmd != NULL);

        cmd->args_size = 0;
        cmd->args_cap = ARGS_DEFAULT_CAP;

        cmd->args = malloc(cmd->args_cap * sizeof(char *));
        assert(cmd->args != NULL);

        for (size_t i = 0; input[i] != '\0'; ++i) {
            char buf[100] = {0};

            size_t j = 0;
            while (input[i] != ' ' && input[i] != '\0') {
                assert(i < 100 - 1);
                buf[j++] = input[i++];
            }

            if (cmd->args_size == cmd->args_cap) {
                cmd->args_cap *= 2;
                cmd->args = realloc(cmd->args, cmd->args_cap * sizeof(char *));
                assert(cmd->args != NULL);
            }

            char *arg = malloc((strlen(buf) + 1) * sizeof(char));
            assert(arg != NULL);

            strcpy(arg, buf);
            cmd->args[cmd->args_size++] = arg;
        }

        cmd->args[cmd->args_size++] = NULL;

        cmd->name = malloc((strlen(cmd->args[0]) + 1) * sizeof(char));
        assert(cmd->name != NULL);
        strcpy(cmd->name, cmd->args[0]);

        if (strcmp(cmd->name, "exit") == 0) {
            shell->exit = 1;
        } else if (strcmp(cmd->name, "path") == 0) {
            shell->paths_size = 0;
            if (cmd->args[1] == NULL) {
                strcpy(shell->paths[shell->paths_size++], "");
                continue;
            }

            for (size_t i = 1; i < cmd->args_size - 1; ++i) {
                assert(i < 5);
                assert(strlen(cmd->args[i]) < 100);
                strcpy(shell->paths[shell->paths_size++], cmd->args[i]);
            }
        } else if (strcmp(cmd->name, "cd") == 0) {
            char *dir_name = cmd->args[1];
            if (dir_name == NULL) {
                fprintf(stderr, "No such file or directory\n");
                continue;
            }

            if (chdir(dir_name) < 0) {
                fprintf(stderr, "%s\n", strerror(errno));
                continue;
            }
        } else {
            char path[100] = {0};
            assert(strlen(shell->paths[0]) + strlen(cmd->name) < 100);
            sprintf(path, "%s/%s", shell->paths[0], cmd->name);

            pid_t cid = fork();

            if (cid < 0) {
                fprintf(stderr, "Could not create child process\n");
                continue;
            }

            if (cid == 0) {
                execv(path, cmd->args);
                fprintf(stderr, "unreachable!\n");
                exit(1);
            }

            waitpid(cid, NULL, 0);
        }

        for (size_t i = 0; cmd->args[i] != NULL; ++i) {
            free(cmd->args[i]);
        }
        free(cmd->args);
        free(cmd->name);
        free(cmd);
    }

    free(shell);

    return 0;
}
