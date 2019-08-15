#include "common.h"
#include "circle_queue_safe.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

PCircleQueue circlequeue_init(int max_size)
{
    PCircleQueue p;

    p = calloc(sizeof(CircleQueue), 1);
    if (NULL == p)
    {
        DBG_PRINT("calloc fail");

        return NULL;
    }

    p->head = p->tail = p->data_size = 0;
    p->max_size = max_size;

    if (0 != pthread_rwlock_init(&(p->rwlock), NULL))
    {
        DBG_PRINT("pthread_rwlock_init fail");

        return NULL;
    }

    p->data = calloc(sizeof(ele), p->max_size);
    if (NULL == p->data)
    {
        DBG_PRINT("calloc fail");

        return NULL;
    }

    return p;
}

bool circlequeue_is_full(PCircleQueue p)
{
    assert(NULL != p);

    return p->data_size == p->max_size;
}

bool circlequeue_is_empty(PCircleQueue p)
{
    assert(NULL != p);

    return 0 == p->data_size;
}

void circlequeue_print(PCircleQueue p)
{
    assert(NULL != p);

    int i = 0;
    int index;
    for (; i < p->data_size; i++)
    {
        index = (p->head + i) % p->max_size;

        printf("index=%d, data=%d \n", index, p->data[index]);
    }
}

void circlequeue_free(PCircleQueue p)
{
    assert(NULL != p);

    if (NULL != p->data)
    {
        free(p->data);
    }

    pthread_rwlock_destroy(&(p->rwlock));

    free(p);
    p = NULL;
}

int circlequeue_enqueue_safe(PCircleQueue p, ele val)
{
    assert(NULL != p);

    pthread_rwlock_wrlock(&(p->rwlock));

    if (circlequeue_is_full(p))
    {
        DBG_PRINT("queue is full");

        return -1;
    }

    p->data[p->tail] = val;

    p->data_size++;

    p->tail += 1;
    p->tail = p->tail % p->max_size;

#ifdef _DEBUG
    printf("circlequeue_enqueue: data_size=%d, tail=%d \n", p->data_size, p->tail);
#endif

    pthread_rwlock_unlock(&(p->rwlock));

    return 0;
}

/*
    从队列头部出

    success: 返回0
    fail：
      -1 队列满了
*/
int circlequeue_dequeue_safe(PCircleQueue p, ele* pEle)
{
    assert(NULL != p);
    assert(NULL != pEle);

    pthread_rwlock_wrlock(&(p->rwlock));

    if (circlequeue_is_empty(p))
    {
        DBG_PRINT("queue is empty");

        return -1;
    }

    // 取出队列头部的数据返回
    if (NULL != pEle)
    {
        *pEle = p->data[p->head];
    }

    // 清空当前位置的数据
    p->data[p->head] = 0;

    // 有数据出队，相关标识更新
    p->data_size--;

    p->head += 1;
    p->head = p->head % p->max_size;

    pthread_rwlock_unlock(&(p->rwlock));

    return 0;
}