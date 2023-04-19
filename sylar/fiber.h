/**
* @file: fiber.h
* @brief: 协程
* @author: Kinvy.Qiu
* @email: Kinvy66@163.com
* @date: 2023/4/19 
*/
#ifndef __SYLAR_FIBER_H
#define __SYLAR_FIBER_H

#include <memory>
#include <functional>
#include <ucontext.h>
#include "thread.h"

namespace sylar {

class Fiber: public std::enable_shared_from_this<Fiber> {
public:
    std::shared_ptr<Fiber> ptr;

    enum State {
        INIT,
        HOLD,
        EXEC,
        TERM,
        READY
    };

private:
    Fiber();

public:
    Fiber(std::function<void() cb>, size_t stacksize = 0);
    ~Fiber();

    void reset(std::function<void()> cb);

};

}


#endif //__SYLAR_FIBER_H
