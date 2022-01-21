#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

void change_var(void)
{
    int x = 100;

    pid_t child_pid = fork();

    if (child_pid < 0) {
        fprintf(stderr, "could not create child process\n");
        exit(1);
    } else if (child_pid == 0) {
        x += 13;
    } else {
        x += 8;
    }

    printf("pid: %i, x: %i\n", child_pid, x);
}

void opens_file(void)
{
    int fd = open("temp", O_RDWR | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);

    pid_t child_pid = fork();

    if (child_pid < 0) {
        fprintf(stderr, "could not create child process\n");
        exit(1);
    } else if (child_pid == 0) {
        printf("child fd: %i\n", fd);
        char *buf = "This is\n";
        write(fd, buf, strlen(buf));
    } else {
        printf("parent fd: %i\n", fd);
        char *buf = "test message\n";
        write(fd, buf, strlen(buf));
    }

    close(fd);
}

void greet_farewell(void)
{
    pid_t child_pid = fork();

    if (child_pid < 0) {
        fprintf(stderr, "could not create child process\n");
        exit(1);
    } else if (child_pid == 0) {
        printf("hello\n");
    } else {
        sleep(1);
        printf("goodbye\n");
    }
}

void exec_ls(void)
{
    pid_t child_pid = fork();

    if (child_pid < 0) {
        fprintf(stderr, "could not create child process\n");
        exit(1);
    } else if (child_pid == 0) {
        //execl("/bin/ls", "/bin/ls", "-s", NULL);
        //char *const env[] = {"BLOCKSIZE=69", NULL};
        //execle("/bin/ls", "/bin/ls", "-s", NULL, env);
        execlp("ls", "ls", "-l", NULL);
    } else {
        sleep(1);
        printf("lol\n");
    }
}

void use_wait(void)
{
    pid_t child_pid = fork();

    if (child_pid < 0) {
        fprintf(stderr, "could not create child process\n");
        exit(1);
    } else if (child_pid == 0) {
        printf("hello\n");
    } else {
        wait(NULL);
        printf("goodbye\n");
    }
}

void use_waitpid(void)
{
    pid_t child_pid = fork();

    if (child_pid < 0) {
        fprintf(stderr, "could not create child process\n");
        exit(1);
    } else if (child_pid == 0) {
        printf("hello\n");
    } else {
        waitpid(child_pid, NULL, 0);
        printf("goodbye\n");
    }
}

void close_stdout(void)
{
    pid_t child_pid = fork();

    if (child_pid < 0) {
        fprintf(stderr, "could not create child process\n");
        exit(1);
    } else if (child_pid == 0) {
        close(STDOUT_FILENO);
        printf("hello\n");
    } else {
        wait(NULL);
        printf("goodbye\n");
    }
}

void pipe_two_children(void)
{
    int pipefd[2];
    pipe(pipefd);

    pid_t first = fork();
    if (first == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        write(STDOUT_FILENO, "hello from first\n", 18);
    } else {
        pid_t second = fork();
        if (second == 0) {
            write(STDOUT_FILENO, "hello from second\n", 18);
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            char ch[1];
            while (read(STDIN_FILENO, &ch, 1) > 0) {
                write(STDOUT_FILENO, ch, 1);
            }
        } else {
            close(pipefd[0]);
            close(pipefd[1]);
            waitpid(second, NULL, 0);
            printf("goodbye from parent\n");
        }
    }
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
