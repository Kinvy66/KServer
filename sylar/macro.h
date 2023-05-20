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
#include <cassert>
#include "util.h"

#if defined __GNUC__ || defined __llvm__
#define SYLAR_LICKLY(x)         __builtin_expect(!!(x), 1)
#define SYLAR_UNLICKLY(x)         __builtin_expect(!!(x), 0)
#else
#define SYLAR_LICKLY(x)         (x)
#define SYLAR_UNLICKLY(x)         (x)
#endif

#define SYLAR_ASSERT(x) \
    if(SYLAR_LICKLY(!(x))) { \
        SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "ASSERTION: " #x \
            << "\nbacktrace:\n" \
            << sylar::BacktraceToString(100, 2, "    "); \
        assert(x); \
    }

#define SYLAR_ASSERT2(x, w) \
    if(SYLAR_UNLICKLY(!(x))) { \
        SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "ASSERTION: " #x \
            << "\n" << w                                      \
            << "\nbacktrace:\n"                               \
            << sylar::BacktraceToString(100, 2, "    ");      \
        assert(x); \
    }

#endif //__SYLAR_MACRO_H
