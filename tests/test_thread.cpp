/**
* @file: test_thread.cpp
* @brief:
* @author: Kinvy.Qiu
* @email: Kinvy66@163.com
* @date: 2023/4/16 14:52
*/

#include "sylar/sylar.h"
#include <unistd.h>

sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();

int count = 0;
sylar::RWMutex s_mutex;

void fun1() {
    SYLAR_LOG_INFO(g_logger) << "name: " << sylar::Thread::GetName()
                            << " this.name: " << sylar::Thread::GetThis()->GetName()
                            << " id: " << sylar::GetThreadId()
                            << " this.id: " << sylar::Thread::GetThis()->getId();
    for (int i = 0; i < 100000; ++i) {
        sylar::RWMutex::WriteLock lock(s_mutex);
        ++count;
    }
}

void fun2() {

}

int main(int argc, char** argv) {

    SYLAR_LOG_INFO(g_logger) << "thread test begin";

    std::vector<sylar::Thread::ptr> thrs;

    for(int i = 0; i < 5; ++i) {
        sylar::Thread::ptr thr(new sylar::Thread(&fun1, "name_"+std::to_string(i)));
        thrs.push_back(thr);
    }

    for (int i = 0; i < 5; ++i) {
        thrs[i]->join();
    }
    SYLAR_LOG_INFO(g_logger) << "thread test end";
    SYLAR_LOG_INFO(g_logger) << "count=" << count;

    return 0;
}

// 回去t_thread_name = thread->getName();加上，还有Thread构造函数里没赋值/