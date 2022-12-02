#include <stdio.h>
#include <stdlib.h>
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
    CMD_QUIT,
    CMD_JOBS,
    CMD_BG,
    CMD_FG
};

struct arg {
    char *start;
    size_t len;
};

struct argv {
    size_t size;
    size_t cap;
    struct arg *buf;
};

struct cmd {
    enum cmd_kind kind;
    struct argv argv;
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
    while (fread(&ch, 1, 1, stdin) > 0 && ch != '\n') {
        if (ferror(stdin) != 0) {
            fprintf(stderr, "failed to read from stdin\n");
            exit(1);
        }
        put_char(inp, ch);
    }
}

void put_arg(struct cmd *cmd, char *start, size_t len)
{
    MEM_GROW(&cmd->argv, struct arg);
    cmd->argv.buf[cmd->argv.size++] = (struct arg) {
        .start = start,
        .len = len
    };
}

void parse_input(struct input *inp, struct cmd *cmd)
{
    char *start;
    size_t len, i;
    struct arg *a;

    cmd->argv.size = 0;
    for (i = 0; i < inp->size; ++i) {
        start = inp->buf + i;
        while (inp->buf[i] != ' ' && i < inp->size) i++;
        len = inp->buf + i - start;
        put_arg(cmd, start, len);
    }

    cmd->kind = CMD_NOT_BUILTIN;
    a = cmd->argv.buf;
    switch (a->len) {
    case 4:
        if (memcmp(a->start, "quit", 4) == 0) {
            cmd->kind = CMD_QUIT;
        } else if (memcmp(a->start, "jobs", 4) == 0) {
            cmd->kind = CMD_JOBS;
        }
    case 2:
        if (memcmp(a->start, "bg", 2) == 0) {
            cmd->kind = CMD_BG;
        } else if (memcmp(a->start, "fg", 2) == 0) {
            cmd->kind = CMD_FG;
        }
    }
}

int main(void)
{
    struct input input;
    struct cmd cmd;

    MEM_ALLOC(&input, 5, char);          // @Leak(art): let OS free it
    MEM_ALLOC(&cmd.argv, 2, struct arg); // @Leak(art): let OS free it

    for (;;) {
        printf("tsh> ");

        read_input(&input);
        if (input.size == 0) continue; // @Todo(art): trim input

        parse_input(&input, &cmd);

        switch (cmd.kind) {
        case CMD_QUIT: exit(0);
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
            printf("handling not build in `%.*s` command...\n",
                   cmd.argv.buf->len, cmd.argv.buf->start);
        }
    }

    return 0;
}
