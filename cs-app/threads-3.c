#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 2

char **ptr;

void *thread(void *vargp)
{
    int myid = *((int *) vargp);
    free(vargp);

    static int cnt = 0;

    printf("[%d]: %s (cnt=%d)\n", myid, ptr[myid], ++cnt);

    return NULL;
}

int main(void)
{
    char *msgs[N] = {"Hello from foo", "Hello from bar"};

    ptr = msgs;

    for (size_t i = 0; i < N; ++i) {
        int *arg = malloc(sizeof(int));
        *arg = i;

        pthread_t tid;
        pthread_create(&tid, NULL, thread, arg);
    }

    pthread_exit(NULL);
}

