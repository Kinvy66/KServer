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

    /**
     * @brief 协程状态
     */
    enum State {
        INIT,       // 协程刚创建，没有执行任何代码
        HOLD,       // 挂起状态，不会被调度执行
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

    /**
     * @brief  重置协程函数， 并重置状态为 INIT
     * @param cb 协程任务
     * @details 协程 TERM EXCEPT INIT 状态才能被重置
     */
    void reset(std::function<void()> cb);

    /**
     * @brief 切换到协程执行 EXEC
     */
    void swapIn();

    /**
     * @brief 切换协程到后台
     * @details 由Yield调用，在状态在Yield中设置
     */
    void swapOut();

    void call();
    void back();

    /**
     * @brief 获取协程id
     */
    uint64_t getId() const { return m_id; }

    /**
     * @brief 获取协程状态
     */
    State getState() const { return m_state; }

public:
    /**
     * @brief 更新当前运行协程变量的值
     * @details 当前执行协程的指针保存在线程静态变量t_fiber
     */
    static void SetThis(Fiber* f);

    /**
     * @brief 获取当前协程
     * @details
     *      如果当前线程中没有协程，会自动创建一个主协程
     */
    static Fiber::ptr GetThis();

    /**
     * @brief 让出执行权，协程切换到后台，并且设置为Read状态
     * @note Read 状态协程可以再此被调用
     */
    static void YieldToRead();

    /**
     * @brief 让出执行权，协程切换到后台，并且设置为Hold状态
     * @note Hold状态的协程不会被调度，需要重新手动加入调度
     */
    static void YieldToHold();

    /**
     * @brief 获取总协程数
     */
    static uint64_t TotalFibers();

    /**
     * @brief 协程入口函数
     */
    static void MainFunc();

    /**
     * @brief
     */
    static void CallerMainFunc();

    /**
     * @brief 获取协程id
     * @return
     */
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
    // 协程任务函数
    std::function<void()> m_cb;

};

}


#endif //__SYLAR_FIBER_H
