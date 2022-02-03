#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <pthread.h>

#define N 3

void *handle_thread(void *vargp)
{
    FILE *f = fopen((char *) vargp, "r");
    assert(f != NULL);

    char *buf = calloc(5 * 1024, sizeof(char));
    assert(buf != NULL);

    size_t i = 0;
    char c;
    while ((c = getc(f)) != EOF) {
        buf[i++] = c;
    }

    fclose(f);

    return buf;
}

int main(void)
{
    void *result;
    char *args[N] = {"dir-read.c", "threads-1.c", "threads-2.c"};
    pthread_t tids[N] = {0};

    for (size_t i = 0; i < N; ++i) {
        if (pthread_create(&tids[i], NULL, handle_thread, args[i]) != 0) {
            fprintf(stderr, "Could not create thread\n");
            exit(1);
        }
    }

    for (size_t i = 0; i < N; ++i) {
        if (pthread_join(tids[i], &result) != 0) {
            fprintf(stderr, "Could not join thread\n");
            exit(1);
        }
        printf("File: %s\n", args[i]);
        printf("-----------------\n");
        printf("%s\n", (char *) result);
        free(result);
    }

    return 0;
}
