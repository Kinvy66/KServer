/**
* @file: noncopyable.h
* @brief: 
* @author: Kinvy.Qiu
* @email: Kinvy66@163.com
* @date: 2023/5/11 
*/
#ifndef __SYLAR_NONCOPYABLE_H
#define __SYLAR_NONCOPYABLE_H

namespace sylar {

class Noncopyable {
public:
    Noncopyable() = default;
    ~Noncopyable() = default;
    Noncopyable(const Noncopyable&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;
};

}


#endif //__SYLAR_NONCOPYABLE_H
