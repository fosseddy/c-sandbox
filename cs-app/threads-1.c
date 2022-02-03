#include <stdio.h>
#include <pthread.h>

void *handle_thread(void *vargp)
{
    printf("Hello, world. From thread.\n");
    return NULL;
}

int main(void)
{
    pthread_t tid;

    pthread_create(&tid, NULL, handle_thread, NULL);
    pthread_join(tid, NULL);

    return 0;
}
