#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

int main(void)
{
    struct addrinfo *addr, hints;
    int err;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG | AI_NUMERICSERV;

    if ((err = getaddrinfo("localhost", "3000", &hints, &addr)) > 0) {
        fprintf(stderr, "%s\n", gai_strerror(err));
        exit(1);
    }

    int sockfd;
    int optval = 1;
    for (struct addrinfo *i = addr; i != NULL; i = i->ai_next) {
        sockfd = socket(i->ai_family, i->ai_socktype, i->ai_protocol);

        if (sockfd < 0) continue;

        setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *) &optval, sizeof(int));

        if (bind(sockfd, i->ai_addr, i->ai_addrlen) == 0) break;

        close(sockfd);
        sockfd = -1;
    }

    freeaddrinfo(addr);

    if (sockfd < 0) {
        fprintf(stderr, "%s\n", strerror(errno));
        exit(1);
    }

    if (listen(sockfd, 1024) < 0) {
        fprintf(stderr, "%s\n", strerror(errno));
        exit(1);
    }

    struct sockaddr_storage cliaddr;
    while (1) {
        socklen_t cliaddrlen = sizeof(struct sockaddr_storage);
        int connfd = accept(sockfd, (struct sockaddr *) &cliaddr, &cliaddrlen);
        if (connfd < 0) {
            fprintf(stderr, "%s\n", strerror(errno));
            exit(1);
        }

        write(connfd, "HTTP/1.1 200 OK\r\n", 17);
        write(connfd, "Server: CServer\r\n", 17);
        write(connfd, "Content-Type: text/html\r\n", 25);
        write(connfd, "\r\n", 2);
        write(connfd, "<h1>hello, world</h1>\r\n", 23);

        close(connfd);
    }

    close(sockfd);

    return 0;
}

int main2(void)
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
    write(connfd, "\r\n", 2);
    write(connfd, "hello, world\r\n", 14);

    close(connfd);

    close(sockfd);

    return 0;
}
