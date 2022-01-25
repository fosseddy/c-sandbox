#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define INPUT_CAP 1000

#define ARGS_DEFAULT_CAP 8

//static char *built_in_cmds[] = { "exit", "cd", "path" };

struct Cmd {
    char *name;

    char **args;
    size_t args_cap;
    size_t args_size;
};

struct Shell {
    char *paths[10];
    size_t paths_size;

    int exit;
};

int main(void)
{
    struct Shell *shell = malloc(sizeof(struct Shell));
    shell->exit = 0;
    shell->paths_size = 0;

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

        cmd->args[cmd->args_size] = NULL;

        cmd->name = malloc((strlen(cmd->args[0]) + 1) * sizeof(char));
        assert(cmd->name != NULL);
        strcpy(cmd->name, cmd->args[0]);

        if (strcmp(cmd->name, "exit") == 0) {
            shell->exit = 1;
        } else if (strcmp(cmd->name, "path") == 0) {
            printf("built in path command\n");
        } else if (strcmp(cmd->name, "cd") == 0) {
            printf("built in cd command\n");
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
