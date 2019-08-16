/**
 *      pthread_cond_wait会有很多线程在此阻塞，
 *
 *      1、等消费线程全部启动起来再启动生产线程
 *      2、消费线程启动后（启动好的、没启动好的），马上启动生产线程
 *
 *      3、先启动生产线程
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "include/TaskPool.h"

typedef struct
{
    PTask   pTask;
    int     data_size;
    int     data_max_size;

    int     head;
    int     tail;

    pthread_cond_t  cond_is_full;
    pthread_cond_t  cond_is_empty;

    pthread_mutex_t mutex_lock;

}Data, *PData;

void data_insert2(PData pData, int start, int end)
{
    for (int i = start; i < end; ++i) {
        pthread_mutex_lock(&(pData->mutex_lock));

        while (pData->data_size == pData->data_max_size) {
            sleep(1);

            printf("[thread:%d]taskpool is full \n", pthread_self());

            pthread_cond_wait(&(pData->cond_is_full), &(pData->mutex_lock));
        }

        printf("[thread:%d]有空位置，插入数据: %d \n", pthread_self(), i);

//        pData->pTask[pData->tail] = i;

        pData->data_size += 1;

        pData->tail = (pData->tail + 1) % pData->data_max_size;

        pthread_cond_broadcast(&(pData->cond_is_empty));

        pthread_mutex_unlock(&(pData->mutex_lock));
    }
}

void data_insert(PData pData, void* (*function)(void*), void* arg)
{
    pthread_mutex_lock(&(pData->mutex_lock));

    while (pData->data_size == pData->data_max_size) {
        sleep(1);

        printf("[thread:%d]taskpool is full \n", pthread_self());

        pthread_cond_wait(&(pData->cond_is_full), &(pData->mutex_lock));
    }

    printf("[thread:%d]位置 %d 是空的，插入数据: %d \n", pthread_self(), pData->tail, (int)arg);

    pData->pTask[pData->tail].function = function;
    pData->pTask[pData->tail].arg = arg;

    pData->data_size += 1;

    pData->tail = (pData->tail + 1) % pData->data_max_size;

    pthread_cond_broadcast(&(pData->cond_is_empty));

    pthread_mutex_unlock(&(pData->mutex_lock));
}

void* thread_test(void* arg)
{
    printf("[task:%d]val=%d \n", pthread_self(), arg);

    return NULL;
}

void* thread_produce1(void* arg)
{
    PData pData = (PData)arg;

    data_insert2(pData, 11, 19);

    return NULL;
}

void* thread_produce2(void* arg)
{
    PData pData = (PData)arg;

    data_insert2(pData, 21, 29);


    return NULL;
}

void* thread_produce3(void* arg)
{
    PData pData = (PData)arg;

    data_insert2(pData, 31, 39);

    return NULL;
}

void* thread_produce4(void* arg)
{
    PData pData = (PData)arg;

    data_insert2(pData, 41, 49);

    return NULL;
}

void* thread_consume(void* arg)
{
    PData pData = (PData)arg;
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

        // 执行(执行完线程就退出了，很奇怪)
        (*(task.function))(task.arg);

        pthread_t tid;

        pthread_attr_t thread_attr;

        // 设置线程分离属性
//        if (0 != pthread_attr_init(&thread_attr))
//        {
//            perror("pthread_attr_init() fail");
//
//            break;
//        }
//
//        if (0 != pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_DETACHED))
//        {
//            perror("pthread_attr_setdetachstate() fail");
//
//            break;
//        }
//
//        pthread_create(&tid, &thread_attr, task.function, task.arg);
//
//        pthread_attr_destroy(&thread_attr);
    }

    return NULL;
}

int main(void)
{
//    PTaskPool ptp;
    pthread_t tid1, tid2, tid3, tid4;
    pthread_t consume1, consume2, consume3, consume4;

    Data data;

    data.data_size = 0;
    data.head = 0;
    data.tail = 0;
    data.data_max_size = 2;
    data.pTask = calloc(sizeof(Task), data.data_max_size);

    if (0 != pthread_cond_init(&(data.cond_is_full), NULL)
            || 0 != pthread_cond_init(&(data.cond_is_empty), NULL)
            || 0 != pthread_mutex_init(&(data.mutex_lock), NULL)) {
        perror("init mutex、cond fail");

        exit(-1);
    }

//    ptp = taskpool_init(10);

//    taskpool_insert(ptp, thread_consume, (void*)11);
//    taskpool_insert(ptp, thread_consume, (void*)22);
//    taskpool_insert(ptp, thread_consume, (void*)33);



    pthread_create(&tid1, NULL, thread_consume, (void*)&data);
    pthread_create(&tid2, NULL, thread_consume, (void*)&data);
//    pthread_create(&tid3, NULL, thread_consume, (void*)&data);
//    pthread_create(&tid4, NULL, thread_consume, (void*)&data);

//    pthread_create(&consume1, NULL, thread_produce1, (void*)&data);
//    pthread_create(&consume2, NULL, thread_produce2, (void*)&data);
//    pthread_create(&consume3, NULL, thread_produce3, (void*)&data);
//    pthread_create(&consume4, NULL, thread_produce4, (void*)&data);


    sleep(2);

    for (int i = 0; i < 30; i++) {
        data_insert(&data, thread_test, i + 1);
    }

//    data_insert(&data, thread_test, 1);
//    data_insert(&data, thread_test, 2);
//    data_insert(&data, thread_test, 3);
//    data_insert(&data, thread_test, 4);

    sleep(5);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
//    pthread_join(tid3, NULL);
//    pthread_join(tid4, NULL);

//    for (int i = 0; i < 10; ++i) {
//        taskpool_insert(ptp, thread_test, (void*)i);
//
//        sleep(1);
//    }

    printf("end \n");

//    getchar();

    pthread_cond_destroy(&(data.cond_is_full));
    pthread_cond_destroy(&(data.cond_is_empty));

    pthread_mutex_destroy(&(data.mutex_lock));

//    taskpool_destroy(ptp);

    return 0;
}

