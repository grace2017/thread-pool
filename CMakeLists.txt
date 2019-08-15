cmake_minimum_required(VERSION 3.14)
project(pthread_pool3 C)

set(CMAKE_C_STANDARD 99)

set(TEST_CQ_SRC_LIST test_cq.c include/circle_queue.c)
set(TEST_SAFE_CQ_SRC_LIST test_cq_safe.c include/circle_queue_safe.c)

#add_executable(pthread_pool ${TEST_CQ_SRC_LIST})
add_executable(pthread_pool ${TEST_CQ_SAFE_SRC_LIST})