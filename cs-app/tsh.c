#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MEM_ALLOC(mem, capacity, type)              \
do {                                                \
    (mem)->size = 0;                                \
    (mem)->cap = (capacity);                        \
    (mem)->buf = malloc((mem)->cap * sizeof(type)); \
    if ((mem)->buf == NULL) {                       \
        perror("malloc failed");                    \
        exit(1);                                    \
    }                                               \
} while (0)

#define MEM_GROW(mem, type)                                          \
do {                                                                 \
    if ((mem)->size == (mem)->cap) {                                 \
        (mem)->cap *= 2;                                             \
        (mem)->buf = realloc((mem)->buf, (mem)->cap * sizeof(type)); \
        if ((mem)->buf == NULL) {                                    \
            perror("realloc failed");                                \
            exit(1);                                                 \
        }                                                            \
    }                                                                \
} while (0)

struct input {
    size_t size;
    size_t cap;
    char *buf;
};

enum cmd_kind {
    CMD_NOT_BUILTIN = 0,
    CMD_EXIT,
    CMD_JOBS,
    CMD_BG,
    CMD_FG
};

struct argv {
    size_t size;
    size_t cap;
    char **buf;
};

struct cmd {
    enum cmd_kind kind;
    struct argv argv;
    int bg;
};

void put_char(struct input *inp, char ch)
{
    MEM_GROW(inp, char);
    inp->buf[inp->size++] = ch;
}

void read_input(struct input *inp)
{
    char ch;

    inp->size = 0;
    while (fread(&ch, 1, 1, stdin) > 0 && ch != '\n') put_char(inp, ch);

    if (ferror(stdin)) {
        fprintf(stderr, "failed to read from stdin\n");
        exit(1);
    }
}

void put_arg(struct argv *argv, char *start, size_t len)
{
    char *arg;

    MEM_GROW(argv, char *);

    if (start == NULL) {
        argv->buf[argv->size++] = NULL;
        return;
    }

    if ((arg = calloc(len + 1, 1)) == NULL) {
        perror("malloc failed");
        exit(1);
    }

    strncpy(arg, start, len);
    argv->buf[argv->size++] = arg;
}

void parse_input(struct input *inp, struct cmd *cmd)
{
    char *start, ch;
    size_t len, i;

    for (i = 0; i < cmd->argv.size; ++i) free(cmd->argv.buf[i]);
    cmd->bg = 0;
    cmd->argv.size = 0;

    for (i = 0; i < inp->size; ++i) {
        start = inp->buf + i;
        while (i < inp->size && inp->buf[i] != ' ') i++;
        len = inp->buf + i - start;
        if (len == 0) continue;

        if (len == 1 && *start == '&') {
            cmd->bg = 1;
            break;
        }

        put_arg(&cmd->argv, start, len);
    }

    if (cmd->argv.size == 0) return;

    put_arg(&cmd->argv, NULL, 0);
    cmd->kind = CMD_NOT_BUILTIN;

    if (strcmp(cmd->argv.buf[0], "exit") == 0) {
        cmd->kind = CMD_EXIT;
    } else if (strcmp(cmd->argv.buf[0], "jobs") == 0) {
        cmd->kind = CMD_JOBS;
    } else if (strcmp(cmd->argv.buf[0], "bg") == 0) {
        cmd->kind = CMD_BG;
    } else if (strcmp(cmd->argv.buf[0], "fg") == 0) {
        cmd->kind = CMD_FG;
    }
}

void exec_cmd(struct cmd *cmd)
{
    pid_t pid;

    if ((pid = fork()) < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) {
        if (access(cmd->argv.buf[0], F_OK) < 0) {
            fprintf(stderr, "command `%s` does not exist\n", cmd->argv.buf[0]);
            exit(1);
        }

        if (execve(cmd->argv.buf[0], cmd->argv.buf, NULL) < 0) {
            perror("could not execute command");
            exit(1);
        }
    }

    // parent
    // @Todo(art): add job to joblist
}

int main(void)
{
    struct input input;
    struct cmd cmd;

    MEM_ALLOC(&input, 50, char);     // @Leak(art): let OS free it
    MEM_ALLOC(&cmd.argv, 5, char *); // @Leak(art): let OS free it

    for (;;) {
        printf("tsh> ");

        read_input(&input);
        parse_input(&input, &cmd);

        if (cmd.argv.size == 0) continue;

        switch (cmd.kind) {
        case CMD_EXIT: exit(0);
        case CMD_JOBS:
            printf("handling `jobs` command...\n");
            continue;
        case CMD_BG:
            printf("handling `bg` command...\n");
            continue;
        case CMD_FG:
            printf("handling `fg` command...\n");
            continue;
        default:
            exec_cmd(&cmd);
            wait(NULL);
        }
    }

    return 0;
}
