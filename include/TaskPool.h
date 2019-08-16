#ifndef PTHREAD_POOL3_TASKPOOL_H
#define PTHREAD_POOL3_TASKPOOL_H

#include "circle_queue.h"

#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>

typedef struct
{
    void*   (* function)(void *);
    void*   arg;
}Task, *PTask;

typedef struct
{
    PCircleQueue    pcq;

    pthread_cond_t  cond_is_empty;
    pthread_mutex_t mutex_is_empty;

    pthread_cond_t  cond_is_full;
    pthread_mutex_t mutex_is_full;
}TaskPool, *PTaskPool;

PTaskPool taskpool_init(int pool_max_size);
void taskpool_destroy(PTaskPool ptp);

bool taskpoll_is_empty(PTaskPool ptp);
bool taskpool_is_full(PTaskPool ptp);

int taskpool_insert(PTaskPool ptp, void* (* function)(void *), void* arg);

int taskpool_getone(PTaskPool ptp, ele** pval);



#endif //PTHREAD_POOL3_TASKPOOL_H
