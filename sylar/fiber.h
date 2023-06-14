/**
* @file fiber.h
* @brief 协程封装
* @author Kinvy
* @email Kinvy66@163.com
* @date 2023-4-19
*/
#ifndef __SYLAR_FIBER_H
#define __SYLAR_FIBER_H

#include <memory>
#include <functional>
#include <ucontext.h>
#include "thread.h"

namespace sylar {

class Scheduler;

/**
 * @brief 协程类
 */
class Fiber: public std::enable_shared_from_this<Fiber> {
friend class Scheduler;
public:
    typedef std::shared_ptr<Fiber> ptr;

    /**
     * @brief 协程状态
     */
    enum State {
        /// 初始化状态，协程刚创建，没有执行任何代码
        INIT,
        /// 暂停状态，挂起状态，不会被调度执行
        HOLD,
        /// 执行中状态
        EXEC,
        /// 结束状态
        TERM,
        /// 可执行状态
        READY,
        /// 异常状态
        EXCEPT
    };

private:
    /**
     * @brief 构造函数
     * @attention 每个线程第一个协程的构造
     * 无参构造函数只用于创建线程的第一个协程，也就是线程主函数对应的协程，
     *  这个协程只能由GetThis()方法调用，所以定义成私有方法
     */
    Fiber();

public:
    /**
     * @brief 构造函数，用于创建用户协程
     * @param[in] cb 协程执行的函数
     * @param[in] stacksize 协程栈大小
     * @param[in] use_caller 是否在MainFiber上调度
     */
    Fiber(std::function<void()> cb, size_t stacksize = 0, bool use_caller = false);

    /**
     * @brief 析构函数
     */
    ~Fiber();

    /**
     * @brief  重置协程函数， 并重置状态为 INIT
     * @param cb 协程任务
     * @pre getState() 为 INIT, TERM, EXCEPT
     * @post getState() = INIT
     * @details 协程 TERM EXCEPT INIT 状态才能被重置
     */
    void reset(std::function<void()> cb);

    /**
     * @brief 将当前协程切换到运行状态
     * @pre getState() != EXEC
     * @post getState() = EXEC
     */
    void swapIn();

    /**
     * @brief 将当前协程切换到后台
     * @details 由Yield调用，在状态在Yield中设置
     */
    void swapOut();

    /**
     * @brief 将当前线程切换到执行状态
     * @pre 执行的为当前线程的主协程
     */
    void call();

    /**
     * @brief 将当前线程切换到后台
     * @pre 执行的为该协程
     * @post 返回到线程的主协程
     */
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
     * @brief 设置当前线程的运行协程
     * @param[in] f 运行协程
     * @details 当前执行协程的指针保存在线程静态变量t_fiber
     */
    static void SetThis(Fiber* f);

    /**
     * @brief 获取当前协程
     * @attention
     *      如果当前线程中没有协程，该函数会创建一个主协程
     */
    static Fiber::ptr GetThis();

    /**
     * @brief 让出执行权，协程切换到后台，并且设置为Read状态
     * @post getState() = READY
     */
    static void YieldToReady();

    /**
     * @brief 将当前协程切换到后台,并设置为HOLD状态
     * @post getState() = HOLD
     */
    static void YieldToHold();

    /**
     * @brief 返回当前协程的总数量
     */
    static uint64_t TotalFibers();

    /**
     * @brief 协程执行函数
     * @post 执行完成返回到线程主协程
     */
    static void MainFunc();

    /**
     * @brief 协程执行函数
     * @post 执行完成返回到线程调度协程
     */
    static void CallerMainFunc();

    /**
     * @brief 获取当前协程的id
     */
    static uint64_t  GetFiberId();

private:
    /// 协程id
    uint64_t m_id = 0;
    /// 协程栈大小
    uint32_t m_stacksize = 0;
    /// 协程状态
    State m_state = INIT;
    /// 协程上下文
    ucontext_t m_ctx;
    /// 协程运行栈指针
    void * m_stack = nullptr;
    /// 协程任务函数
    std::function<void()> m_cb;
};

}

#endif //__SYLAR_FIBER_H
