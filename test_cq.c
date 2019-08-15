#include <stdio.h>

#include "include/circle_queue.h"

int main(void)
{
    int data[10];
    PCircleQueue pCQueue;
    ele val;

    pCQueue = circlequeue_init(5);

    circlequeue_enqueue(pCQueue, 1);
    circlequeue_enqueue(pCQueue, 2);
    circlequeue_enqueue(pCQueue, 3);
    circlequeue_enqueue(pCQueue, 4);
    circlequeue_enqueue(pCQueue, 5);
    circlequeue_enqueue(pCQueue, 6);

    circlequeue_print(pCQueue);

    //================
    circlequeue_dequeue(pCQueue, &val);
    printf("出队的数据：%d \n", val);

    circlequeue_dequeue(pCQueue, &val);
    printf("出队的数据：%d \n", val);

    circlequeue_print(pCQueue);

    //================
    circlequeue_enqueue(pCQueue, 7);
    circlequeue_enqueue(pCQueue, 8);
    circlequeue_enqueue(pCQueue, 9);

    circlequeue_print(pCQueue);

    //================
    circlequeue_free(pCQueue);

    getchar();

    return 0;
}