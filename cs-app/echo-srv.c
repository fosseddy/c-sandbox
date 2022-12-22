#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define MAXLINE 255
#define CONN_EOF 0x4

int main(void)
{
    struct addrinfo hints, *addr, *info;
    struct sockaddr_storage cliaddr;
    socklen_t clilen;
    char *host = "127.0.1.1", *port = "3000", buf[MAXLINE];
    int rc, sockfd, clifd, optval = 1;
    ssize_t n;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE|AI_ADDRCONFIG|AI_NUMERICSERV;

    if ((rc = getaddrinfo(host, port, &hints, &info)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rc));
        return 1;
    }

    for (addr = info; addr != NULL; addr = addr->ai_next) {
        sockfd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        if (sockfd < 0) continue;

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR,
                       &optval, sizeof(int)) < 0) {
            close(sockfd);
            continue;
        }

        if (bind(sockfd, addr->ai_addr, addr->ai_addrlen) == 0) break;

        close(sockfd);
        sockfd = -1;
    }

    if (sockfd < 0) {
        fprintf(stderr, "failed to open and bind socket\n");
        return 1;
    }

    freeaddrinfo(info);

    if (listen(sockfd, 1024) < 0) {
        perror("listen failed");
        return 1;
    }

    for (;;) {
        clilen = sizeof(struct sockaddr_storage);
        clifd = accept(sockfd, (struct sockaddr *) &cliaddr, &clilen);
        if (clifd < 0) {
            perror("accept failed");
            continue;
        }

        while ((n = read(clifd, buf, MAXLINE)) != 0 && buf[0] != CONN_EOF) {
            write(clifd, buf, n);
        }

        close(clifd);
    }

    return 0;
}
