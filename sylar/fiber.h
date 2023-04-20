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
    typedef std::shared_ptr<Fiber> ptr;

    enum State {
        INIT,
        HOLD,
        EXEC,
        TERM,
        READY,
        EXCEPT
    };

private:
    Fiber();

public:
    Fiber(std::function<void()> cb, size_t stacksize = 0);
    ~Fiber();

    // 重置协程函数， 并重置状态
    // INIT TERM
    void reset(std::function<void()> cb);
    // 切换到协程执行
    void swapIn();
    // 切换到后台执行
    void swapOut();

    uint64_t getId() const { return m_id; }

public:
    //  设置当前协程
    static void SetThis(Fiber* f);
    // 返回当前协程
    static Fiber::ptr GetThis();
    // 协程切换到后台，并且设置为Read状态
    static void YieldToRead();
    // 协程切换到后台，并且设置为Hold状态
    static void YieldToHold();

    // 总协程数
    static uint64_t TotalFibers();

    static void MainFunc();
    static uint64_t  GetFiberId();

private:
    uint64_t m_id = 0;
    uint32_t m_stacksize = 0;
    State m_state = INIT;

    ucontext_t m_ctx;
    void * m_stack = nullptr;

    std::function<void()> m_cb;

};

}


#endif //__SYLAR_FIBER_H
