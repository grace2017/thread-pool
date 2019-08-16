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
    PTask   pTask;              /* 模拟环形队列 */
    int     data_size;          /* 当前任务数 */
    int     data_max_size;      /* 最大任务数 */

    int     head;               /* 始终指向队列的第一个元素 */
    int     tail;               /* 始终指向队列的最后一个元素 */

    pthread_cond_t  cond_is_full;
    pthread_cond_t  cond_is_empty;

    pthread_mutex_t mutex_lock;

}TaskPool, *PTaskPool;

int taskpool_init(PTaskPool pTaskPool, int max_size);

void taskpool_destroy(PTaskPool pTaskPool);


#endif //PTHREAD_POOL3_TASKPOOL_H
