#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>

pid_t Fork(void)
{
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }
    return pid;
}

int Open(const char *pathname, int flags, mode_t mode)
{
    int fd = open(pathname, flags, mode);
    if (fd < 0) {
        perror("open failed");
        exit(1);
    }
    return fd;
}

void Close(int fd)
{
    if (close(fd) < 0) {
        perror("close failed");
        exit(1);
    }
}

ssize_t Write(int fd, const void *buf, size_t count)
{
    ssize_t b = write(fd, buf, count);
    if ((int) b == -1) {
        perror("write failed");
        exit(1);
    }
    return b;
}


ssize_t Read(int fd, void *buf, size_t count)
{
    ssize_t r = read(fd, buf, count);
    if ((int) r == -1) {
        perror("read failed");
        exit(1);
    }
    return r;
}

void Execve(const char *pathname, char *const argv[], char *const envp[])
{
    if (execve(pathname, argv, envp) < 0) {
        perror("execve failed");
        exit(1);
    }
}

pid_t Wait(int *wstatus)
{
    pid_t p = wait(wstatus);
    if (p < 0 && errno != ECHILD) {
        perror("wait failed");
        exit(1);
    }
    return p;
}

pid_t Waitpid(pid_t pid, int *wstatus, int options)
{
    pid_t p = waitpid(pid, wstatus, options);
    if (p < 0) {
        perror("waitpid failed");
        exit(1);
    }
    return p;
}

void Pipe(int pipefd[2])
{
    if (pipe(pipefd) < 0) {
        perror("pipe failed");
        exit(1);
    }
}

void change_var(void)
{
    int x = 100;

    pid_t pid = Fork();
    if (pid == 0) {
        x += 13;
    } else {
        x += 8;
    }

    printf("pid: %i, x: %i\n", pid, x);
}

void opens_file(void)
{
    int fd = Open("temp", O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
    pid_t pid = Fork();

    if (pid == 0) {
        printf("child fd: %i\n", fd);
        char *buf = "This is\n";
        Write(fd, buf, strlen(buf));
    } else {
        printf("parent fd: %i\n", fd);
        char *buf = "test message\n";
        Write(fd, buf, strlen(buf));
    }

    Close(fd);
}

void greet_farewell(void)
{
    pid_t pid = Fork();

    if (pid == 0) {
        printf("hello\n");
    } else {
        sleep(1);
        printf("goodbye\n");
    }
}

void exec_ls(void)
{
    pid_t pid = Fork();

    if (pid == 0) {
        char *argv[] = {"/bin/ls", "-la", NULL};
        Execve(argv[0], argv, NULL);
    } else {
        Wait(NULL);
    }
}

void use_wait(void)
{
    pid_t pid = Fork();

    if (pid == 0) {
        Wait(NULL);
        printf("hello\n");
    } else {
        Wait(NULL);
        printf("goodbye\n");
    }
}

void use_waitpid(void)
{
    pid_t pid = Fork();

    if (pid == 0) {
        printf("hello\n");
    } else {
        Waitpid(pid, NULL, 0);
        printf("goodbye\n");
    }
}

void close_stdout(void)
{
    pid_t pid = Fork();

    if (pid == 0) {
        Close(STDOUT_FILENO);
        printf("hello\n");
    } else {
        Wait(NULL);
        printf("goodbye\n");
    }
}

void pipe_two_children(void)
{
    int pfd[2];
    Pipe(pfd);

    pid_t pid1 = Fork();
    if (pid1 == 0) {
        printf("this is child 1 (%d)\n", (int) getpid());
        Close(pfd[0]);
        char buf[30] = {0};
        sprintf(buf, "hello from %d\n", (int) getpid());
        Write(pfd[1], buf, strlen(buf));
        exit(0);
    }

    pid_t pid2 = Fork();
    if (pid2 == 0) {
        printf("this is child 2 (%d)\n", (int) getpid());
        Close(pfd[1]);
        char c;
        while (Read(pfd[0], &c, 1)) {
            fputc(c, stdout);
        }
        exit(0);
    }

    Close(pfd[0]);
    Close(pfd[1]);
    while (Wait(NULL) > 0);
}

int main(void)
{
    //change_var();
    //opens_file();
    //greet_farewell();
    //exec_ls();
    //use_wait();
    //use_waitpid();
    //close_stdout();
    pipe_two_children();

    return 0;
}
