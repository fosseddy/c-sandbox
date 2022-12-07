#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>

#define BUFIO_BUFCAP (4<<10)

struct file_t {
    int fd;
    int eof;
    size_t size;
    size_t cursor;
    char buf[BUFIO_BUFCAP];
};

int bufio_eof(struct file_t *f)
{
    return f->eof && f->cursor == f->size;
}

struct file_t *bufio_open(char *path, int flags, mode_t mode)
{
    struct file_t *f;

    if ((f = malloc(sizeof(struct file_t))) == NULL) return NULL;

    if ((f->fd = open(path, flags, mode)) < 0) {
        free(f);
        return NULL;
    }

    f->cursor = 0;
    f->size = 0;
    f->eof = 0;

    return f;
}

ssize_t bufio_read(struct file_t *f, void *ptr, size_t count)
{
    assert(f->cursor <= f->size);

    size_t i = 0;

    while (!bufio_eof(f) && i < count) {
        if (f->cursor == f->size) {
            f->size = read(f->fd, f->buf, BUFIO_BUFCAP);
            if (f->size < 0) return -1;
            if (f->size < BUFIO_BUFCAP) f->eof = 1;
            f->cursor = 0;
        }

        ((char *) ptr)[i++] = f->buf[f->cursor++];
    }

    return i;
}

int bufio_close(struct file_t *f)
{
    int fd = f->fd;

    free(f);
    return close(fd);
}

#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    struct file_t *f;
    char ch;

    f = bufio_open(argv[1], O_RDONLY, 0);

    while (bufio_read(f, &ch, 1) > 0) putchar(ch);

    bufio_close(f);
    return 0;
}
