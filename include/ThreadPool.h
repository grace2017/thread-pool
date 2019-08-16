#ifndef PTHREAD_POOL3_THREADPOOL_H
#define PTHREAD_POOL3_THREADPOOL_H

#include "common.h"
#include "TaskPool.h"

#include <pthread.h>

typedef struct
{
    pthread_t   manage_thread;
    pthread_t*  threads;

    int         thread_min_size;
    int         thread_max_size;
    int         thread_busy_size;
    int         thread_live_size;

    pthread_mutex_t     mutex_lock;
}ThreadPool, *PThreadPool;

PThreadPool threadpool_init(int min_size, int max_size, PTaskPool pTaskPool);

void threadpool_join(PThreadPool pThreadPool);

void threadpool_destroy(PThreadPool pThreadPool);

#endif //PTHREAD_POOL3_THREADPOOL_H
