#ifndef PTHREAD_POOL3_CIRCLE_QUEUE_SAFE_H
#define PTHREAD_POOL3_CIRCLE_QUEUE_SAFE_H

#include <stdbool.h>
#include <pthread.h>

#define ele int

typedef struct
{
    int   head;       /* 队列头部 */
    int   tail;       /* 队列尾部 */
    int   max_size;   /* 最大数据数 */
    int   data_size;  /* 实时数据数 */
    ele*  data;

    pthread_rwlock_t  rwlock;   /* 利用读写锁实现线程安全 */
}CircleQueue, *PCircleQueue;

PCircleQueue circlequeue_init(int max_size);
bool circlequeue_is_full(PCircleQueue p);
bool circlequeue_is_empty(PCircleQueue p);
void circlequeue_print(PCircleQueue p);
void circlequeue_free(PCircleQueue p);

int circlequeue_enqueue_safe(PCircleQueue p, ele val);
int circlequeue_dequeue_safe(PCircleQueue p, ele* pEle);

#endif //PTHREAD_POOL3_CIRCLE_QUEUE_SAFE_H
