#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
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

struct input_t {
    size_t size;
    size_t cap;
    char *buf;
};

enum cmdkind_t {
    CMD_NOT_BUILTIN = 0,
    CMD_EXIT,
    CMD_JOBS,
    CMD_BG,
    CMD_FG
};

struct argv_t {
    size_t size;
    size_t cap;
    char **buf;
};

struct cmd_t {
    enum cmdkind_t kind;
    struct argv_t argv;
    int bg;
};

enum jobstatus_t {
    JSTAT_FG = 0,
    JSTAT_BG,
    JSTAT_ST
};

struct job_t {
    int id;
    pid_t pid;
    char *name;
    enum jobstatus_t status;
};

struct joblist_t {
    size_t size;
    size_t cap;
    struct job_t *buf;
};

extern char **environ;
struct joblist_t joblist;
int job_id = 1;

void unix_error(char *msg)
{
    perror(msg);
    exit(1);
}

void put_char(struct input_t *input, char ch)
{
    MEM_GROW(input, char);
    input->buf[input->size++] = ch;
}

void read_input(struct input_t *input)
{
    char ch;

    input->size = 0;
    while (fread(&ch, 1, 1, stdin) > 0 && ch != '\n') put_char(input, ch);

    if (ferror(stdin)) {
        fprintf(stderr, "failed to read from stdin\n");
        exit(1);
    }
}

void put_arg(struct argv_t *argv, char *start, size_t len)
{
    char *arg;

    MEM_GROW(argv, char *);

    if (start == NULL) {
        argv->buf[argv->size++] = NULL;
        return;
    }

    if ((arg = calloc(len + 1, 1)) == NULL) unix_error("calloc failed");

    strncpy(arg, start, len);
    argv->buf[argv->size++] = arg;
}

void parse_input(struct input_t *input, struct cmd_t *cmd)
{
    char *start, ch;
    size_t len, i;

    for (i = 0; i < cmd->argv.size; ++i) free(cmd->argv.buf[i]);
    cmd->bg = 0;
    cmd->argv.size = 0;

    for (i = 0; i < input->size; ++i) {
        while (input->buf[i] == ' ') i++;

        start = input->buf + i;
        while (i < input->size && input->buf[i] != ' ') i++;

        len = input->buf + i - start;
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

void put_job(pid_t pid, char *name, int isbg)
{
    struct job_t job;

    job.id = job_id++;
    job.pid = pid;
    job.status = JSTAT_FG;
    if (isbg) job.status = JSTAT_BG;

    if ((job.name = malloc(strlen(name) + 1)) == NULL) {
        unix_error("malloc failed");
    }
    strcpy(job.name, name);

    MEM_GROW(&joblist, struct job_t);
    joblist.buf[joblist.size++] = job;
}

void showjobs()
{
    size_t i;

    for (i = 0; i < joblist.size; ++i) {
        struct job_t j = joblist.buf[i];

        printf("[%d] (%d) ", j.id, j.pid);

        switch (j.status) {
        case JSTAT_FG: printf("Foreground "); break;
        case JSTAT_BG: printf("Running "); break;
        case JSTAT_ST: printf("Stopped "); break;
        }

        printf("%s\n", j.name);
    }
}

void exec_cmd(struct cmd_t *cmd)
{
    sigset_t set, oldset;
    pid_t pid;

    sigaddset(&set, SIGTSTP);
    sigprocmask(SIG_BLOCK, &set, &oldset);

    if ((pid = fork()) < 0) unix_error("fork failed");

    if (pid == 0) {
        sigprocmask(SIG_SETMASK, &oldset, NULL);

        if (setpgid(0, 0) < 0) unix_error("setpgid failed");

        if (access(cmd->argv.buf[0], F_OK) < 0) {
            fprintf(stderr, "command `%s` does not exist\n", cmd->argv.buf[0]);
            exit(1);
        }

        if (execve(cmd->argv.buf[0], cmd->argv.buf, environ) < 0) {
            unix_error("could not execute command");
        }
    }

    put_job(pid, cmd->argv.buf[0], cmd->bg);
    sigprocmask(SIG_SETMASK, &oldset, NULL);
}

void sigtstp_handler(int sig)
{
    char *msg, *err;
}

int main(void)
{
    struct sigaction sigtstp_act;
    struct input_t input;
    struct cmd_t cmd;

    sigtstp_act.sa_handler = sigtstp_handler;
    sigtstp_act.sa_flags = SA_RESTART;
    sigemptyset(&sigtstp_act.sa_mask);

    if (sigaction(SIGTSTP, &sigtstp_act, NULL) < 0) {
        perror("signal setting failed");
        exit(1);
    }

    MEM_ALLOC(&input, 50, char);           // @Leak(art): let OS free it
    MEM_ALLOC(&cmd.argv, 5, char *);       // @Leak(art): let OS free it
    MEM_ALLOC(&joblist, 30, struct job_t); // @Leak(art): let OS free it

    for (;;) {
        printf("tsh> ");

        read_input(&input);
        parse_input(&input, &cmd);

        if (cmd.argv.size == 0) continue;

        switch (cmd.kind) {
        case CMD_EXIT: exit(0);
        case CMD_JOBS:
            showjobs();
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
