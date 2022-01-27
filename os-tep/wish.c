#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <assert.h>

#define INPUT_CAP 1000

#define ARGS_CAP 8

#define PATHS_CAP 4
#define DEFAULT_PATH "/bin"

#define REALLOC_ARR(arr, cap, type)             \
    do {                                        \
        cap *= 2;                               \
        arr = realloc(arr, cap * sizeof(type)); \
        assert(arr != NULL);                    \
    } while (0)                                 \

enum Cmd_Kind {
    NOT_BUILT_IN = 0,
    BUILT_IN_EXIT,
    BUILT_IN_PATH,
    BUILT_IN_CD,
    CMD_KIND_LENGTH,
};

struct Cmd {
    char **args;
    size_t args_cap;
    size_t args_size;

    enum Cmd_Kind kind;

    char *redirect_dest;
    int redirect;
};

struct Shell {
    char **paths;
    size_t paths_cap;
    size_t paths_size;

    int exit;
};

static char *built_in_cmds[CMD_KIND_LENGTH] = {
    [NOT_BUILT_IN]  = "",
    [BUILT_IN_CD]   = "cd",
    [BUILT_IN_EXIT] = "exit",
    [BUILT_IN_PATH] = "path",
};

struct Shell *make_shell(void);
struct Cmd *make_cmd(void);
size_t shell_parse_cmds(char *, struct Cmd **);
char *strdup(char *);

int main(void)
{
    struct Shell *shell = make_shell();

    while (!shell->exit) {
        char input[INPUT_CAP] = {0};
        fgets(input, INPUT_CAP, stdin);
        input[strlen(input) - 1] = '\0';

        if (strlen(input) == 0) continue;

        struct Cmd *buf[10] = {0};
        size_t sz = shell_parse_cmds(input, buf);

        struct Cmd *cmd = buf[0];
        if (cmd->kind == NOT_BUILT_IN) {
            for (size_t i = 0; i < sz; ++i) {
                struct Cmd *cmd = buf[i];
                char *cmd_name = cmd->args[0];
                char path[100] = {0};

                int path_exist = 0;
                for (size_t j = 0; j < shell->paths_size; ++j) {
                    assert(strlen(shell->paths[j]) + strlen(cmd_name) < 100);
                    sprintf(path, "%s/%s", shell->paths[j], cmd_name);
                    if ((path_exist = access(path, X_OK)) == 0) break;
                }

                if (path_exist < 0) {
                    fprintf(stderr, "command `%s` not found\n", cmd_name);
                } else {
                    pid_t cid = fork();

                    if (cid < 0) {
                        fprintf(stderr, "Could not create child process\n");
                    } else if (cid == 0) {
                        if (cmd->redirect) {
                            int fd;
                            if ((fd = open(cmd->redirect_dest, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR)) < 0) {
                                fprintf(stderr, "%s\n", strerror(errno));
                            } else {
                                dup2(fd, STDOUT_FILENO);
                                dup2(fd, STDERR_FILENO);
                                close(fd);
                            }
                        }

                        execv(path, cmd->args);
                        assert(0 && "Unreachable!\n");
                    }
                }
            }

            while (waitpid(-1, NULL, 0) > 0);
        } else {
            switch (cmd->kind) {
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

                default: assert(0 && "Unreachable\n");
            }
        }

        for (size_t i = 0; i < sz; ++i) {
            for (size_t j = 0; buf[i]->args[j] != NULL; ++j) {
                free(buf[i]->args[j]);
            }
            free(buf[i]->args);
            free(buf[i]->redirect_dest);
            free(buf[i]);
        }
    }

    for (size_t i = 0; i < shell->paths_size; ++i) {
        free(shell->paths[i]);
    }
    free(shell->paths);
    free(shell);

    return 0;
}

struct Shell *make_shell(void)
{
    struct Shell *shell = malloc(sizeof(struct Shell));
    assert(shell != NULL);

    shell->exit = 0;
    shell->paths_size = 0;
    shell->paths_cap = PATHS_CAP;

    shell->paths = malloc(shell->paths_cap * sizeof(char *));
    assert(shell->paths != NULL);

    shell->paths[shell->paths_size++] = strdup(DEFAULT_PATH);

    return shell;
}

struct Cmd *make_cmd(void)
{
    struct Cmd *cmd = malloc(sizeof(struct Cmd));
    assert(cmd != NULL);

    cmd->redirect = 0;
    cmd->redirect_dest = NULL;

    cmd->args_size = 0;
    cmd->args_cap = ARGS_CAP;

    cmd->args = malloc(cmd->args_cap * sizeof(char *));
    assert(cmd->args != NULL);

    cmd->kind = NOT_BUILT_IN;

    return cmd;
}

size_t shell_parse_cmds(char *input, struct Cmd **arr)
{
    size_t size = 0;
    char *tok = strtok(input, " ");
    while (tok != NULL) {
        struct Cmd *cmd = make_cmd();

        while (tok != NULL) {
            if (strcmp(tok, ">") == 0) {
                cmd->redirect = 1;
                char *dest = strtok(NULL, " ");
                if (dest == NULL) {
                    fprintf(stderr, "Provide redirect destination\n");
                } else {
                    cmd->redirect_dest = strdup(dest);
                }
            } else if (strcmp(tok, "&") == 0) {
                tok = strtok(NULL, " ");
                break;
            } else {
                if (cmd->args_size + 1 == cmd->args_cap) {
                    REALLOC_ARR(cmd->args, cmd->args_cap, char *);
                }

                cmd->args[cmd->args_size++] = strdup(tok);
            }

            tok = strtok(NULL, " ");
        }

        cmd->args[cmd->args_size++] = NULL;

        for (enum Cmd_Kind i = 1; i < CMD_KIND_LENGTH; ++i) {
            if (strcmp(cmd->args[0], built_in_cmds[i]) == 0) {
                cmd->kind = i;
                break;
            }
        }

        arr[size++] = cmd;
    }

    return size;
}

char *strdup(char *src)
{
    char *p = malloc((strlen(src) + 1) * sizeof(char));
    assert(p != NULL);

    strcpy(p, src);

    return p;
}
