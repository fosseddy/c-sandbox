#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(void)
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, "%s\n", strerror(errno));
        exit(1);
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(6969);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (bind(sockfd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        fprintf(stderr, "%s\n", strerror(errno));
        exit(1);
    }

    if (listen(sockfd, 1024) < 0) {
        fprintf(stderr, "%s\n", strerror(errno));
        exit(1);
    }

    struct sockaddr_in cliaddr;
    socklen_t cliaddrlen = sizeof(cliaddr);

    int connfd = accept(sockfd, (struct sockaddr *) &cliaddr, &cliaddrlen);
    if (connfd < 0) {
        fprintf(stderr, "%s\n", strerror(errno));
        exit(1);
    }

    write(connfd, "HTTP/1.1 200 OK\r\n", 17);
    write(connfd, "Server: CServer\r\n", 17);
    write(connfd, "Content-Type: text/plain\r\n", 26);
    write(connfd, "Connection: close\r\n", 19);
    write(connfd, "\r\n", 2);
    write(connfd, "hello, world\r\n", 14);

    close(connfd);

    close(sockfd);

    return 0;
}
