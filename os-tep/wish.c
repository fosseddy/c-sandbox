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

enum Built_In_Kind {
    NOT_BUILT_IN = 0,
    BUILT_IN_EXIT,
    BUILT_IN_PATH,
    BUILT_IN_CD,
    LENGTH_OF_BUILT_IN,
};

static char *built_in_cmds[] = {
    [NOT_BUILT_IN] = "",
    [BUILT_IN_EXIT] = "exit",
    [BUILT_IN_PATH] = "path",
    [BUILT_IN_CD] = "cd"
};

struct Cmd {
    char **args;
    size_t args_cap;
    size_t args_size;

    enum Built_In_Kind built_in_kind;
};

struct Shell {
    char paths[5][100];
    size_t paths_size;

    int exit;
};

struct Shell *make_shell(void)
{
    struct Shell *shell = malloc(sizeof(struct Shell));
    assert(shell != NULL);

    shell->exit = 0;
    shell->paths_size = 0;

    strcpy(shell->paths[shell->paths_size++], "/bin");

    return shell;
}

int main(void)
{
    struct Shell *shell = make_shell();

    while (!shell->exit) {
        char input[INPUT_CAP] = {0};
        fgets(input, INPUT_CAP, stdin);
        input[strlen(input) - 1] = '\0';

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
        cmd->built_in_kind = NOT_BUILT_IN;

        char *cmd_name = cmd->args[0];

        for (enum Built_In_Kind i = 0; i < LENGTH_OF_BUILT_IN; ++i) {
            if (strcmp(cmd_name, built_in_cmds[i]) == 0) {
                cmd->built_in_kind = i;
                break;
            }
        }

        if (cmd->built_in_kind == NOT_BUILT_IN) {
            char path[100] = {0};
            assert(strlen(shell->paths[0]) + strlen(cmd_name) < 100);
            sprintf(path, "%s/%s", shell->paths[0], cmd_name);

            pid_t cid = fork();

            if (cid < 0) {
                fprintf(stderr, "Could not create child process\n");
            } else if (cid == 0) {
                execv(path, cmd->args);
                fprintf(stderr, "child process %i: unreachable!\n", cid);

                for (size_t i = 0; cmd->args[i] != NULL; ++i) {
                    free(cmd->args[i]);
                }
                free(cmd->args);
                free(cmd);
                free(shell);

                exit(1);
            }

            waitpid(cid, NULL, 0);
        } else {
            switch (cmd->built_in_kind) {
                case BUILT_IN_EXIT:
                    shell->exit = 1;
                    break;

                case BUILT_IN_PATH:
                    shell->paths_size = 0;
                    if (cmd->args[1] == NULL) {
                        strcpy(shell->paths[shell->paths_size++], "");
                    } else {
                        for (size_t i = 1; i < cmd->args_size - 1; ++i) {
                            assert(i < 5);
                            assert(strlen(cmd->args[i]) < 100);
                            strcpy(shell->paths[shell->paths_size++], cmd->args[i]);
                        }
                    }
                    break;

                case BUILT_IN_CD: {
                    char *dir_name = cmd->args[1];
                    if (dir_name == NULL) {
                        fprintf(stderr, "No such file or directory\n");
                    } else if (chdir(dir_name) < 0) {
                        fprintf(stderr, "%s\n", strerror(errno));
                    }
                } break;

                default: assert(0 && "Unreachable\n");
            }
        }

        for (size_t i = 0; cmd->args[i] != NULL; ++i) {
            free(cmd->args[i]);
        }
        free(cmd->args);
        free(cmd);
    }

    free(shell);

    return 0;
}
