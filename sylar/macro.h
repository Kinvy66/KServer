/**
* @file: macro.h
* @brief: 常用宏
* @author: Kinvy.Qiu
* @email: Kinvy66@163.com
* @date: 2023/4/19 19:14
*/
#ifndef __SYLAR_MACRO_H
#define __SYLAR_MACRO_H

#include <string>
#include <assert.h>
#include "util.h"

#define SYLAR_ASSERT(x) \
    if(!(x)) { \
        SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "ASSERTION: " #x \
            << "\nbacktrace:\n" \
            << sylar::BacktraceToString(100, 2, "    "); \
        assert(x); \
    }

#define SYLAR_ASSERT2(x, w) \
    if(!(x)) { \
        SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "ASSERTION: " #x \
            << "\n" << w                                      \
            << "\nbacktrace:\n"                               \
            << sylar::BacktraceToString(100, 2, "    ");      \
        assert(x); \
    }

#endif //__SYLAR_MACRO_H
