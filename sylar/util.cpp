/**
* @author: Kinvy
* @email: Kinvy66@163.com
* @date: 2023/3/31 18:40
* @file: util.cpp
* @description: 
*/
#include "util.h"

pid_t sylar::GetThreadId() {
    return syscall(SYS_gettid);
}

uint32_t sylar::GetFiberId() {
    return 0;
}