/**
 *      1、等消费线程全部启动起来再启动生产线程
 *      2、消费线程启动后（启动好的、没启动好的），马上启动生产线程
 *
 *      3、先启动生产线程
 *
 *      用到的知识
 *          多线程
 *          线程互斥：mutex
 *          线程同步：条件变量
 *          线程与信号
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "include/TaskPool.h"
#include "include/ThreadPool.h"

void data_insert2(PTaskPool pData, int start, int end)
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

void data_insert(PTaskPool pData, void* (*function)(void*), void* arg)
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
    PTaskPool pData = (PTaskPool)arg;

    data_insert2(pData, 11, 19);

    return NULL;
}

void* thread_produce2(void* arg)
{
    PTaskPool pData = (PTaskPool)arg;

    data_insert2(pData, 21, 29);


    return NULL;
}

void* thread_produce3(void* arg)
{
    PTaskPool pData = (PTaskPool)arg;

    data_insert2(pData, 31, 39);

    return NULL;
}

void* thread_produce4(void* arg)
{
    PTaskPool pData = (PTaskPool)arg;

    data_insert2(pData, 41, 49);

    return NULL;
}

void* thread_consume(void* arg)
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

        // 执行(执行完线程就退出了，很奇怪)
        (*(task.function))(task.arg);
    }

    return NULL;
}

void sig_alarm_handler(int signo)
{
    if (SIGALRM == signo) {
        printf("倒计时信号\n");
    }
}

int main(void)
{
//    PTaskPool ptp;
    pthread_t tid1, tid2, tid3, tid4;
    pthread_t consume1, consume2, consume3, consume4;

    TaskPool data;
    ThreadPool threadPool;

    signal(SIGALRM, sig_alarm_handler);

    alarm(2);

    sleep(15);

//    taskpool_init(&data, 2);

//    threadpool_init(2, 10, &data);

//    这样写无法运行，原因没想明白
//    PTaskPool pTaskPool = taskpool_init(2);

//    pthread_create(&tid1, NULL, thread_consume, (void*)&data);
//    pthread_create(&tid2, NULL, thread_consume, (void*)&data);
//    pthread_create(&tid3, NULL, thread_consume, (void*)&data);
//    pthread_create(&tid4, NULL, thread_consume, (void*)&data);

//    pthread_create(&consume1, NULL, thread_produce1, (void*)&data);
//    pthread_create(&consume2, NULL, thread_produce2, (void*)&data);
//    pthread_create(&consume3, NULL, thread_produce3, (void*)&data);
//    pthread_create(&consume4, NULL, thread_produce4, (void*)&data);


//    sleep(2);
//
//    for (int i = 0; i < 10; i++) {
//        data_insert(&data, thread_test, i + 1);
//    }

//    data_insert(&data, thread_test, 1);
//    data_insert(&data, thread_test, 2);
//    data_insert(&data, thread_test, 3);
//    data_insert(&data, thread_test, 4);

//    for (int i = 0; i < 10; ++i) {
//        taskpool_insert(ptp, thread_test, (void*)i);
//
//        sleep(1);
//    }

    printf("end \n");

//    pthread_join(tid1, NULL);
//    pthread_join(tid2, NULL);
//    pthread_join(tid3, NULL);
//    pthread_join(tid4, NULL);

    // 回收线程池中执行完任务的子线程
//    threadpool_join(&threadPool);

//    taskpool_destroy(&data);

//    threadpool_destroy(&threadPool);

//    taskpool_destroy(ptp);

    return 0;
}

