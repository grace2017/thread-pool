#ifndef PTHREAD_POOL3_COMMON_H
#define PTHREAD_POOL3_COMMON_H

#include <stdbool.h>

#define _DEBUG

#define DBG_PRINT(msg)  do { printf("(%s:%d):%s \n", __FILE__, __LINE__, msg); }while(0)

#endif //PTHREAD_POOL3_COMMON_H
