#include <stdio.h>
#include <pthread.h>

volatile static long count = 0;

void *thread_handler(void *args)
{
    long i, iter = *(long *)args;

    for (i = 0; i < iter; ++i) count++;

    return NULL;
}

int main(void)
{
    pthread_t tid1, tid2;
    long iter = 1000000;

    pthread_create(&tid1, NULL, thread_handler, &iter);
    pthread_create(&tid2, NULL, thread_handler, &iter);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    if (count != iter * 2) {
        printf("fail: %ld\n", count);
        return 1;
    }

    puts("success");
    return 0;
}
