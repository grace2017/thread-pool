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
    int*    data;
    int     data_size;
    int     data_max_size;

    int     head;
    int     tail;

//    int     read_ready;
//    int     write_ready;

    pthread_cond_t  cond_is_full;
    pthread_cond_t  cond_is_empty;

    pthread_mutex_t mutex_is_empty;
    pthread_mutex_t mutex_is_full;

}Data, *PData;

void data_insert(PData pData, int val)
{
    pthread_mutex_lock(&(pData->mutex_is_empty));

    while (pData->data_size == pData->data_max_size) {
        sleep(1);

        printf("[thread:%d]taskpool is full \n", pthread_self());

        pthread_cond_wait(&(pData->cond_is_full), &(pData->mutex_is_empty));
    }

    printf("[thread:%d]有空位置，插入数据: %d \n", pthread_self(), val);

    pData->data[pData->tail] = val;

    pData->data_size += 1;

    pData->tail = (pData->tail + 1) % pData->data_max_size;

    pthread_cond_broadcast(&(pData->cond_is_empty));

    pthread_mutex_unlock(&(pData->mutex_is_empty));
}

void* thread_test(void* arg)
{

}

void* thread_produce(void* arg)
{
    PData pData = (PData)arg;


    return NULL;
}

void* thread_consume(void* arg)
{
    PData pData = (PData)arg;

    while (1) {
        pthread_mutex_lock(&(pData->mutex_is_empty));

        while (0 == pData->data_size) {
            printf("[thread:%d]taskpool is empty \n", pthread_self());

            pthread_cond_wait(&(pData->cond_is_empty), &(pData->mutex_is_empty));
        }

        printf("[thread:%d]读到数据:%d，有空位置了 \n", pthread_self(), pData->data[pData->head]);

        pData->data[pData->head] = 0;

        pData->data_size -= 1;

        pData->head = (pData->head + 1) % pData->data_max_size;

        if (0 != pthread_cond_broadcast(&(pData->cond_is_full))) {
            perror("pthread_cond_broadcast fail");
        }

        pthread_mutex_unlock(&(pData->mutex_is_empty));
    }

    return NULL;
}

int main(void)
{
//    PTaskPool ptp;
    pthread_t tid1, tid2, tid3, tid4;

    Data data;

    data.data_size = 0;
    data.head = 0;
    data.tail = 0;
    data.data_max_size = 2;
    data.data = calloc(sizeof(int), data.data_max_size);

    if (0 != pthread_cond_init(&(data.cond_is_full), NULL)
            || 0 != pthread_cond_init(&(data.cond_is_empty), NULL)
            || 0 != pthread_mutex_init(&(data.mutex_is_full), NULL)
            || 0 != pthread_mutex_init(&(data.mutex_is_empty), NULL)) {
        perror("init mutex、cond fail");

        exit(-1);
    }

//    ptp = taskpool_init(10);

//    taskpool_insert(ptp, thread_consume, (void*)11);
//    taskpool_insert(ptp, thread_consume, (void*)22);
//    taskpool_insert(ptp, thread_consume, (void*)33);



    pthread_create(&tid1, NULL, thread_consume, (void*)&data);
    pthread_create(&tid2, NULL, thread_consume, (void*)&data);
    pthread_create(&tid3, NULL, thread_consume, (void*)&data);
    pthread_create(&tid4, NULL, thread_consume, (void*)&data);

//    sleep(2);

    for (int i = 0; i < 30; ++i) {
        data_insert(&data, i + 1);
    }

//    data_insert(&data, 1);
//    data_insert(&data, 2);
//    data_insert(&data, 3);
//    data_insert(&data, 4);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
    pthread_join(tid4, NULL);

//    for (int i = 0; i < 10; ++i) {
//        taskpool_insert(ptp, thread_test, (void*)i);
//
//        sleep(1);
//    }

    printf("end \n");

//    getchar();

    pthread_cond_destroy(&(data.cond_is_full));
    pthread_cond_destroy(&(data.cond_is_empty));

    pthread_mutex_destroy(&(data.mutex_is_full));
    pthread_mutex_destroy(&(data.mutex_is_empty));

//    taskpool_destroy(ptp);

    return 0;
}

