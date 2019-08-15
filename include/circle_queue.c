#include "common.h"
#include "circle_queue.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <pthread.h>

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
    return p->data_size == p->max_size;
}

bool circlequeue_is_empty(PCircleQueue p)
{
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

        printf("data=%d \n", p->data[index]);
    }
}

void circlequeue_free(PCircleQueue p)
{
    assert(NULL != p);

    if (NULL != p->data)
    {
        free(p->data);
    }

    free(p);
    p = NULL;
}

int circlequeue_enqueue(PCircleQueue p, ele val)
{
    assert(NULL != p);

    if (circlequeue_is_full(p))
    {
        DBG_PRINT("queue is full");

        return -1;
    }

    p->data[p->tail] = val;

    p->data_size++;

    p->tail += 1;
    p->tail = p->tail % p->max_size;

    printf("[thread:%d]circlequeue_enqueue: data_size=%d, tail=%d \n", pthread_self(), p->data_size, p->tail);

    return 0;
}

/*
    从队列头部出

    success: 返回0
    fail：
      -1 队列满了
*/
int circlequeue_dequeue(PCircleQueue p, ele* pEle)
{
    assert(NULL != p);
    assert(NULL != pEle);

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

    return 0;
}