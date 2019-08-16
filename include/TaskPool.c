#include "TaskPool.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

PTaskPool taskpool_init(int pool_max_size)
{
    PTaskPool ptp;

    ptp = calloc(sizeof(TaskPool), 1);

    ptp->pcq = circlequeue_init(pool_max_size);

    pthread_cond_init(&(ptp->cond_is_empty), NULL);
    pthread_cond_init(&(ptp->cond_is_full), NULL);

    pthread_mutex_init(&(ptp->mutex_is_empty), NULL);
    pthread_mutex_init(&(ptp->mutex_is_full), NULL);

    return ptp;
}

void taskpool_destroy(PTaskPool ptp)
{
    pthread_cond_destroy(&(ptp->cond_is_empty));
    pthread_cond_destroy(&(ptp->cond_is_full));

    pthread_mutex_destroy(&(ptp->cond_is_empty));
    pthread_mutex_destroy(&(ptp->cond_is_full));

    circlequeue_free(ptp->pcq);

    free(ptp);
    ptp = NULL;
}

bool taskpoll_is_empty(PTaskPool pcp)
{
    return circlequeue_is_empty(pcp);
}

bool taskpool_is_full(PTaskPool pcp)
{
    return circlequeue_is_full(pcp);
}

int taskpool_insert(PTaskPool ptp, void* (* function)(void *), void* arg)
{
    int ret = 0;
    int errno;

    if (0 != (errno = pthread_mutex_lock(&(ptp->mutex_is_full)))) {
        fprintf(stderr, "%s \n", strerror(errno));

        perror("pthread_mutex_lock fail");

        pthread_exit(-1);
    }

    while (taskpool_is_full(ptp)) {
        printf("[thread:%d]taskpool is full \n", pthread_self());

        pthread_cond_wait(&(ptp->cond_is_full), &(ptp->mutex_is_full));
    }

    PTask pTask = calloc(sizeof(Task), 1);
    pTask->function = function;
    pTask->arg = arg;

    circlequeue_enqueue(ptp->pcq, pTask);

    pthread_cond_broadcast(&(ptp->cond_is_empty));

    pthread_mutex_unlock(&(ptp->mutex_is_full));

    return ret;
}

int taskpool_getone(PTaskPool ptp, ele** pval)
{
    int ret = 0;
    int val;

    circlequeue_dequeue(ptp->pcq, &val);

    *pval = val;

    return ret;
}