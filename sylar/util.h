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
#include <vector>
#include <string>
#include <sys/time.h>

namespace sylar {

pid_t GetThreadId();
uint32_t GetFiberId();

// 获取函数调用栈信息
void Backtrace(std::vector<std::string>& bt, int size = 64, int skip = 1);
std::string BacktraceToString(int size = 64, int skip = 2,  const std::string& prefix = "");

// 时间 ms
uint64_t GetCurrentMS();
uint64_t GetCurrentUS();

}

#endif //__SYLAR_UTIL_H
