/**
* @author: Kinvy
* @email: Kinvy66@163.com
* @date: 2023/3/31 18:33
* @file: util.h
* @description: 
*/
#ifndef __SYLAR_UTIL_H
#define __SYLAR_UTIL_H

#include <pthread.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <cstdio>
#include <unistd.h>
#include <cstdint>

namespace sylar {

pid_t GetThreadId();
uint32_t GetFiberId();

}

#endif //__SYLAR_UTIL_H
