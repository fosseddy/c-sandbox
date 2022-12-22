#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

#define MAXLINE 255

int main(int argc, char **argv)
{
    struct addrinfo hints, *addr, *info;
    int rc, sockfd;
    char buf[MAXLINE];

    if (argc < 3) {
        fprintf(stderr, "usage: ./c <host> <port>\n");
        return 1;
    }

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_ADDRCONFIG|AI_NUMERICSERV;

    rc = getaddrinfo(argv[1], argv[2], &hints, &info);
    if (rc != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rc));
        return 1;
    }

    for (addr = info; addr != NULL; addr = addr->ai_next) {
        sockfd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        if (sockfd < 0) continue;

        rc = connect(sockfd, addr->ai_addr, addr->ai_addrlen);
        if (rc == 0) break;

        close(sockfd);
        sockfd = -1;
    }

    if (sockfd < 0) {
        fprintf(stderr, "failed to open and connect socket\n");
        return 1;
    }

    freeaddrinfo(info);

    while (fgets(buf, MAXLINE, stdin) != NULL) {
        write(sockfd, buf, strlen(buf));
        read(sockfd, buf, MAXLINE);
        fputs(buf, stdout);
    }

    close(sockfd);
    return 0;
}
