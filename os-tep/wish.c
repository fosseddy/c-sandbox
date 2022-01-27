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
#define CMDS_CAP 4
#define PATHS_CAP 4

#define DEFAULT_EXEC_PATH "/bin"

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
    size_t args_size;

    enum Cmd_Kind kind;

    int redirect;
    char *redirect_dest;
};

static char *built_in_cmds[CMD_KIND_LENGTH] = {
    [NOT_BUILT_IN]  = NULL,
    [BUILT_IN_CD]   = "cd",
    [BUILT_IN_EXIT] = "exit",
    [BUILT_IN_PATH] = "path",
};

size_t parse_input(char *, struct Cmd **);
void free_cmds(struct Cmd *, size_t);
void free_paths(char **, size_t);
char *strdup(char *);

int main(void)
{
    int exit = 0;

    size_t paths_cap = PATHS_CAP;
    size_t paths_size = 0;
    char **paths = malloc(paths_cap * sizeof(char *));
    assert(paths != NULL);

    paths[paths_size++] = strdup(DEFAULT_EXEC_PATH);

    while (!exit) {
        char input[INPUT_CAP] = {0};
        fgets(input, INPUT_CAP, stdin);
        input[strlen(input) - 1] = '\0';

        if (strlen(input) == 0) continue;

        struct Cmd *cmds = NULL;
        size_t cmds_size = parse_input(input, &cmds);

        if (cmds_size > 1) {
            int is_valid = 1;
            for (size_t i = 0; i < cmds_size; ++i) {
                if (cmds[i].kind != NOT_BUILT_IN) {
                    is_valid = 0;
                    break;
                }
            }

            if (!is_valid) {
                fprintf(stderr, "Parallel execution is not allowed "
                                "with built in commands\n");
                free_cmds(cmds, cmds_size);
                continue;
            }
        }

        struct Cmd cmd = cmds[0];
        if (cmd.kind == NOT_BUILT_IN) {
            for (size_t i = 0; i < cmds_size; ++i) {
                struct Cmd cmd = cmds[i];
                char *cmd_name = cmd.args[0];
                char path[100] = {0};

                int path_exist = 0;
                for (size_t j = 0; j < paths_size; ++j) {
                    assert(strlen(paths[j]) + strlen(cmd_name) < 100);
                    sprintf(path, "%s/%s", paths[j], cmd_name);
                    if ((path_exist = access(path, X_OK)) == 0) break;
                }

                if (path_exist < 0) {
                    fprintf(stderr, "command `%s` not found\n", cmd_name);
                } else {
                    pid_t cid = fork();

                    if (cid < 0) {
                        fprintf(stderr, "Could not create child process\n");
                    } else if (cid == 0) {
                        if (cmd.redirect) {
                            int fd;
                            if ((fd = open(cmd.redirect_dest, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR)) < 0) {
                                fprintf(stderr, "%s\n", strerror(errno));
                            } else {
                                dup2(fd, STDOUT_FILENO);
                                dup2(fd, STDERR_FILENO);
                                close(fd);
                            }
                        }

                        execv(path, cmd.args);
                        assert(0 && "Unreachable!\n");
                    }
                }
            }

            while (waitpid(-1, NULL, 0) > 0);
        } else {
            switch (cmd.kind) {
                case BUILT_IN_EXIT:
                    exit = 1;
                    break;

                case BUILT_IN_PATH:
                    for (size_t i = 0; i < paths_size; ++i) {
                        free(paths[i]);
                    }
                    paths_size = 0;
                    if (cmd.args[1] == NULL) {
                        paths[paths_size++] = strdup("");
                    } else {
                        for (size_t i = 1; i < cmd.args_size - 1; ++i) {
                            if (paths_size == paths_cap) {
                                REALLOC_ARR(paths, paths_cap, char *);
                            }
                            paths[paths_size++] = strdup(cmd.args[i]);
                        }
                    }
                    break;

                case BUILT_IN_CD: {
                    char *dir_name = cmd.args[1];
                    if (dir_name == NULL) {
                        fprintf(stderr, "No such file or directory\n");
                    } else if (chdir(dir_name) < 0) {
                        fprintf(stderr, "%s\n", strerror(errno));
                    }
                } break;

                default: assert(0 && "Unreachable\n");
            }
        }

        free_cmds(cmds, cmds_size);
    }

    free_paths(paths, paths_size);

    return 0;
}

size_t parse_input(char *input, struct Cmd **bufptr)
{
    assert(strlen(input) > 0);
    assert(*bufptr == NULL);

    size_t size = 0;
    size_t cap = CMDS_CAP;

    *bufptr = malloc(cap * sizeof(struct Cmd));
    assert(*bufptr != NULL);

    char *tok = strtok(input, " ");
    while (tok != NULL) {
        struct Cmd cmd = (struct Cmd) {
            .redirect = 0,
            .redirect_dest = NULL,

            .args = NULL,
            .args_size = 0,

            .kind = NOT_BUILT_IN
        };

        size_t args_cap = ARGS_CAP;
        cmd.args = malloc(args_cap * sizeof(char *));
        assert(cmd.args != NULL);

        while (tok != NULL) {
            if (strcmp(tok, ">") == 0) {
                char *dest = strtok(NULL, " ");
                if (dest == NULL) {
                    fprintf(stderr, "Provide redirect destination\n");
                } else {
                    cmd.redirect = 1;
                    cmd.redirect_dest = strdup(dest);
                }
            } else if (strcmp(tok, "&") == 0) {
                tok = strtok(NULL, " ");
                break;
            } else {
                if (cmd.args_size + 1 == args_cap) {
                    REALLOC_ARR(cmd.args, args_cap, char *);
                }

                cmd.args[cmd.args_size++] = strdup(tok);
            }

            tok = strtok(NULL, " ");
        }

        cmd.args[cmd.args_size++] = NULL;

        for (enum Cmd_Kind i = 1; i < CMD_KIND_LENGTH; ++i) {
            if (strcmp(cmd.args[0], built_in_cmds[i]) == 0) {
                cmd.kind = i;
                break;
            }
        }

        if (size == cap) {
            REALLOC_ARR(*bufptr, cap, struct Cmd);
        }

        (*bufptr)[size++] = cmd;
    }

    return size;
}

void free_cmds(struct Cmd *cmds, size_t size)
{
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; cmds[i].args[j] != NULL; ++j) {
            free(cmds[i].args[j]);
        }
        free(cmds[i].args);
        free(cmds[i].redirect_dest);
    }

    free(cmds);
}

void free_paths(char **paths, size_t size)
{
    for (size_t i = 0; i < size; ++i) {
        free(paths[i]);
    }

    free(paths);
}

char *strdup(char *src)
{
    char *p = malloc((strlen(src) + 1) * sizeof(char));
    assert(p != NULL);

    strcpy(p, src);

    return p;
}
