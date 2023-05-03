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
class Scheduler;
class Fiber: public std::enable_shared_from_this<Fiber> {
friend class Scheduler;
public:
    typedef std::shared_ptr<Fiber> ptr;

    enum State {
        INIT,       // 协程刚创建，没有执行任何代码
        HOLD,       //
        EXEC,       // 执行中
        TERM,       // 终止
        READY,      // 就绪，协程执行了部分代码
        EXCEPT      // 异常
    };

private:
    /**
     * @brief 构造函数
     * @attention 无参构造函数只用于创建线程的第一个协程，也就是线程主函数对应的协程，
     *  这个协程只能由GetThis()方法调用，所以定义成私有方法
     */
    Fiber();

public:
    /**
     * @brief 构造函数，用于创建用户协程
     * @param cb 协程入口函数
     * @param stacksize 协程栈大小
     * @param user_caller
     */
    Fiber(std::function<void()> cb, size_t stacksize = 0, bool user_caller = false);

    /**
     * @brief 析构函数
     */
    ~Fiber();

    // 重置协程函数， 并重置状态
    // INIT TERM
    void reset(std::function<void()> cb);
    // 切换到协程执行
    void swapIn();
    // 切换到后台执行
    void swapOut();

    void call();
    void back();

    uint64_t getId() const { return m_id; }
    State getState() const { return m_state; }

public:
    //  设置当前协程
    static void SetThis(Fiber* f);

    /**
     * @brief 获取当前协程
     * @details
     *      如果当前线程中没有协程，会自动创建一个主协程
     */
    static Fiber::ptr GetThis();

    /**
     * @brief 让出执行权，协程切换到后台，并且设置为Read状态
     */
    static void YieldToRead();

    /**
     * @brief 让出执行权，协程切换到后台，并且设置为Hold状态
     */
    static void YieldToHold();

    /**
     * @brief 获取总协程数
     */
    static uint64_t TotalFibers();

    static void MainFunc();
    static void CallerMainFunc();
    static uint64_t  GetFiberId();

private:
    // 协程id
    uint64_t m_id = 0;
    // 协程栈大小
    uint32_t m_stacksize = 0;
    // 协程状态
    State m_state = INIT;
    // 协程上下文
    ucontext_t m_ctx;
    // 协程栈地址
    void * m_stack = nullptr;
    // 协程入口函数
    std::function<void()> m_cb;

};

}


#endif //__SYLAR_FIBER_H
