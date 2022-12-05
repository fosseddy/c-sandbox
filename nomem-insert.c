#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char **argv)
{
    int fd;
    char *s, c;
    size_t slen;
    off_t size, end, offset;

    fd = open("nomem-insert.txt", O_RDWR);

    s = argv[1];
    slen = strlen(s);

    offset = atoi(argv[2]);
    size = lseek(fd, 0, SEEK_END) - 1;
    end = size + slen;

    while (size >= offset) {
        lseek(fd, size, SEEK_SET);
        read(fd, &c, 1);

        lseek(fd, end, SEEK_SET);
        write(fd, &c, 1);

        size--;
        end--;
    }

    lseek(fd, offset, SEEK_SET);
    write(fd, s, slen);

    return 0;
}
