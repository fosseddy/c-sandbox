#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAXLINE 1024

struct pool_t {
    int maxfd;
    fd_set read_set;
    fd_set ready_set;
    int readycount;
    int lastidx;
    int fd[FD_SETSIZE];
};

int bytecount = 0;

void echo(int fd)
{
    char buf[1024];
    memset(buf, 0, sizeof(buf));

    read(fd, buf, 1024);
    write(fd, buf, 1024);
}

void init_pool(struct pool_t *p, int fd)
{
    int i;

    p->maxfd = fd;
    p->lastidx = -1;
    FD_ZERO(&p->read_set);
    FD_SET(fd, &p->read_set);

    for (i = 0; i < FD_SETSIZE; ++i) p->fd[i] = -1;
}

void putclient(struct pool_t *p, int fd)
{
    int i;

    p->readycount--;
    for (i = 0; i < FD_SETSIZE; ++i) {
        if (p->fd[i] < 0) {
            p->fd[i] = fd;
            FD_SET(fd, &p->read_set);

            if (p->maxfd < fd) p->maxfd = fd;
            if (p->lastidx < i) p->lastidx = i;

            return;
        }
    }

    fprintf(stderr, "error: too many clients\n");
    exit(1);
}

void serve_clients(struct pool_t *p)
{
    int i, connfd;
    ssize_t n;
    char buf[MAXLINE];

    for (i = 0; i <= p->lastidx && p->readycount > 0; ++i) {
        connfd = p->fd[i];
        if (connfd > 0 && FD_ISSET(connfd, &p->ready_set)) {
            p->readycount--;
            if ((n = read(connfd, buf, MAXLINE)) != 0 && buf[0] != 0x4) {
                bytecount += n;
                printf("Server received %d (%d total) bytes on fd %d\n",
                        n, bytecount, connfd);
                write(connfd, buf, n);
            } else {
                printf("EOF on fd %d\n", connfd);
                close(connfd);
                FD_CLR(connfd, &p->read_set);
                p->fd[i] = -1;
            }
        }
    }
}

//void sigchld_handler(int sig)
//{
//    while (waitpid(-1, 0, WNOHANG) > 0);
//}

int main(void)
{
    int sockfd, connfd;
    struct sockaddr_in addr, cliaddr;
    socklen_t cliaddrlen;
    static struct pool_t pool;

//    signal(SIGCHLD, sigchld_handler);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6969);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    bind(sockfd, (struct sockaddr *) &addr, sizeof(addr));
    listen(sockfd, 1024);

    init_pool(&pool, sockfd);

    for (;;) {
        pool.ready_set = pool.read_set;
        pool.readycount = select(pool.maxfd + 1, &pool.ready_set,
                                 NULL, NULL, NULL);

        if (FD_ISSET(sockfd, &pool.ready_set)) {
            cliaddrlen = sizeof(cliaddr);
            connfd = accept(sockfd, (struct sockaddr *) &cliaddr, &cliaddrlen);
            putclient(&pool, connfd);
        }

        serve_clients(&pool);

        //cliaddrlen = sizeof(cliaddr);
        //connfd = accept(sockfd, (struct sockaddr *) &cliaddr, &cliaddrlen);

        //if (fork() == 0) {
        //    close(sockfd);
        //    echo(connfd);
        //    close(connfd);
        //    exit(0);
        //}

        //close(connfd);
    }

    close(sockfd);
    return 0;
}
