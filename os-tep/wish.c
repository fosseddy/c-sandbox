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

#define PATHS_DEFAULT_CAP 4
#define DEFAUTL_PATH "/bin"

#define REALLOC_ARR(arr, cap, type)             \
    do {                                        \
        cap *= 2;                               \
        arr = realloc(arr, cap * sizeof(type)); \
        assert(arr != NULL);                    \
    } while (0)                                 \

enum Built_In_Kind {
    NOT_BUILT_IN = 0,
    BUILT_IN_EXIT,
    BUILT_IN_PATH,
    BUILT_IN_CD,
    BUILT_IN_DUMP,
    LENGTH_OF_BUILT_IN,
};

static char *built_in_cmds[LENGTH_OF_BUILT_IN] = {
    [NOT_BUILT_IN]  = "",
    [BUILT_IN_CD]   = "cd",
    [BUILT_IN_EXIT] = "exit",
    [BUILT_IN_PATH] = "path",
    [BUILT_IN_DUMP] = "dump"
};

struct Cmd {
    char **args;
    size_t args_cap;
    size_t args_size;

    enum Built_In_Kind built_in_kind;
};

struct Shell {
    char **paths;
    size_t paths_cap;
    size_t paths_size;

    int exit;
};

char *strdup(char *);

int main(void)
{
    struct Shell *shell = malloc(sizeof(struct Shell));
    assert(shell != NULL);

    shell->exit = 0;
    shell->paths_size = 0;
    shell->paths_cap = PATHS_DEFAULT_CAP;

    shell->paths = malloc(shell->paths_cap * sizeof(char *));
    assert(shell->paths != NULL);

    shell->paths[shell->paths_size++] = strdup(DEFAUTL_PATH);

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

        char *tok = strtok(input, " ");
        while (tok != NULL) {
            if (cmd->args_size + 1 == cmd->args_cap) {
                REALLOC_ARR(cmd->args, cmd->args_cap, char *);
            }

            cmd->args[cmd->args_size++] = strdup(tok);

            tok = strtok(NULL, " ");
        }

        cmd->args[cmd->args_size++] = NULL;
        cmd->built_in_kind = NOT_BUILT_IN;

        char *cmd_name = cmd->args[0];

        for (enum Built_In_Kind i = 1; i < LENGTH_OF_BUILT_IN; ++i) {
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
                for (size_t i = 0; i < shell->paths_size; ++i) {
                    free(shell->paths[i]);
                }
                free(shell->paths);
                free(shell);

                exit(1);
            } else {
                waitpid(cid, NULL, 0);
            }
        } else {
            switch (cmd->built_in_kind) {
                case BUILT_IN_EXIT:
                    shell->exit = 1;
                    break;

                case BUILT_IN_PATH:
                    for (size_t i = 0; i < shell->paths_size; ++i) {
                        free(shell->paths[i]);
                    }
                    shell->paths_size = 0;
                    if (cmd->args[1] == NULL) {
                        shell->paths[shell->paths_size++] = strdup("");
                    } else {
                        for (size_t i = 1; i < cmd->args_size - 1; ++i) {
                            if (shell->paths_size == shell->paths_cap) {
                                REALLOC_ARR(shell->paths, shell->paths_cap, char *);
                            }
                            shell->paths[shell->paths_size++] = strdup(cmd->args[i]);
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

                case BUILT_IN_DUMP:
                    printf("cmd:\n");
                    printf("    args_size: %lu\n", cmd->args_size);
                    printf("    args_cap: %lu\n", cmd->args_cap);
                    printf("    args:\n");
                    for (size_t i = 0; i < cmd->args_size; ++i) {
                        printf("        arg %lu: %s\n", i, cmd->args[i]);
                    }

                    printf("\n");

                    printf("shell:\n");
                    printf("    paths_size: %lu\n", shell->paths_size);
                    printf("    paths_cap: %lu\n", shell->paths_cap);
                    printf("    paths:\n");
                    for (size_t i = 0; i < shell->paths_size; ++i) {
                        printf("        path %lu: %s\n", i, shell->paths[i]);
                    }

                    printf("\n");
                    break;

                default: assert(0 && "Unreachable\n");
            }
        }

        for (size_t i = 0; cmd->args[i] != NULL; ++i) {
            free(cmd->args[i]);
        }
        free(cmd->args);
        free(cmd);
    }

    for (size_t i = 0; i < shell->paths_size; ++i) {
        free(shell->paths[i]);
    }
    free(shell->paths);
    free(shell);

    return 0;
}

char *strdup(char *src)
{
    char *p = malloc((strlen(src) + 1) * sizeof(char));
    assert(p != NULL);

    strcpy(p, src);

    return p;
}
