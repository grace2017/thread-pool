#include "TaskPool.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

int taskpool_init(PTaskPool pTaskPool, int max_size)
{
    assert(NULL != pTaskPool);

    int ret = 0;

    pTaskPool->data_size = 0;
    pTaskPool->head = 0;
    pTaskPool->tail = 0;
    pTaskPool->data_max_size = 2;
    pTaskPool->pTask = calloc(sizeof(Task), pTaskPool->data_max_size);

    if (0 != pthread_cond_init(&(pTaskPool->cond_is_full), NULL)
        || 0 != pthread_cond_init(&(pTaskPool->cond_is_empty), NULL)
        || 0 != pthread_mutex_init(&(pTaskPool->mutex_lock), NULL)) {
        perror("init mutex、cond fail");

        exit(-1);
    }

    return ret;
}

void taskpool_destroy(PTaskPool pTaskPool)
{
    if (0 != pthread_cond_destroy(&(pTaskPool->cond_is_empty))
            || 0 != pthread_cond_destroy(&(pTaskPool->cond_is_full))
            || 0 != pthread_mutex_destroy(&(pTaskPool->mutex_lock))) {
        perror("destroy cond or mutex fail");
    }

    if (NULL != pTaskPool->pTask) {
        free(pTaskPool->pTask);

        pTaskPool->pTask = NULL;
    }

    if (NULL != pTaskPool) {
        free(pTaskPool);

        pTaskPool = NULL;
    }
}
