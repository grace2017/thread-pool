#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#include "include/circle_queue_safe.h"

PCircleQueue pCQueue;

void* thread_fun(void* arg)
{
    circlequeue_enqueue_safe(pCQueue, (int)arg);
}

int main(void)
{
    pthread_t tid[10];

    pCQueue = circlequeue_init(9);

    int i=0, j=0;

    for (; i < 10; i++)
    {
        pthread_create(&tid[i], NULL, thread_fun, (void*)i);
    }

    // for (; i < 10; j++)
    // {
    //   pthread_join(tid[i], NULL);
    // }

    sleep(5);

    printf("执行结束 \n");

    circlequeue_print(pCQueue);

    //================
    circlequeue_free(pCQueue);

    sleep(1);

    getchar();

    return 0;
}