#include "ThreadPool.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

void *manage_thread_fn(void* arg) {

}

void* process(void* arg)
{
    PTaskPool pData = (PTaskPool)arg;
    Task task;

    while (1) {
        pthread_mutex_lock(&(pData->mutex_lock));

        while (0 == pData->data_size) {
            printf("[thread:%d]taskpool is empty \n", pthread_self());

            pthread_cond_wait(&(pData->cond_is_empty), &(pData->mutex_lock));
        }

        printf("[thread:%d]位置 %d 读到数据了，有空位置了 \n", pthread_self(), pData->head) ;

        task = pData->pTask[pData->head];

        pData->pTask[pData->head].function = NULL;
        pData->pTask[pData->head].arg = NULL;

        pData->data_size -= 1;

        pData->head = (pData->head + 1) % pData->data_max_size;

        if (0 != pthread_cond_broadcast(&(pData->cond_is_full))) {
            perror("pthread_cond_broadcast fail");
        }

        pthread_mutex_unlock(&(pData->mutex_lock));

        (*(task.function))(task.arg);
    }

    pthread_exit(1);
}

PThreadPool threadpool_init(int min_size, int max_size, PTaskPool pTaskPool)
{
    PThreadPool pThreadPool;

    pThreadPool = calloc(sizeof(ThreadPool), 1);
    if (NULL == pThreadPool) {
        DBG_PRINT("calloc fail");

        goto threadpool_init_end;
    }

    pThreadPool->threads = calloc(sizeof(pthread_t), max_size);
    if (NULL == pThreadPool->threads) {
        DBG_PRINT("calloc fail");

        goto threadpool_init_end;
    }

    if (0 != pthread_mutex_init(&(pThreadPool->mutex_lock), NULL)) {
        DBG_PRINT("calloc fail");

        goto threadpool_init_end;
    }

    pThreadPool->thread_min_size = min_size;
    pThreadPool->thread_max_size = max_size;
    pThreadPool->thread_busy_size = 0;
    pThreadPool->thread_live_size = min_size;

    // 启动线程
    for (int i = 0; i < pThreadPool->thread_min_size; ++i) {
        pthread_create(&(pThreadPool->threads[i]), NULL, process, (void*)pTaskPool);
    }

//    pthread_create(&(pThreadPool->manage_thread), NULL, manage_thread_fn, )

    goto threadpool_init_end;

threadpool_init_fail:
    if (NULL != pThreadPool->threads) {
        free(pThreadPool->threads);
        pThreadPool->threads = NULL;
    }

    if (NULL != pThreadPool) {
        free(pThreadPool);
        pThreadPool = NULL;
    }

threadpool_init_end:
    return pThreadPool;
}

void threadpool_destroy(PThreadPool pThreadPool)
{
    assert(NULL != pThreadPool);

    if (0 != pthread_mutex_destroy(&(pThreadPool->mutex_lock))) {
        printf("destroy mutex fail");
    }

    if (NULL != pThreadPool->threads) {
        free(pThreadPool->threads);

        pThreadPool->threads = NULL;
    }

    if (NULL != pThreadPool) {
        free(pThreadPool);

        pThreadPool = NULL;
    }
}

void threadpool_join(PThreadPool pThreadPool)
{
    int endno;

    for (int i = 0; i < pThreadPool->thread_live_size; ++i) {
        pthread_join(pThreadPool->threads[i], &endno);

        printf("[thread:%d]end:%d \n", pthread_self(), endno);
    }

//    pthread_join(pThreadPool->manage_thread, NULL);
}
