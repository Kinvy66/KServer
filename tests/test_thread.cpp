/**
* @file: test_thread.cpp
* @brief:
* @author: Kinvy.Qiu
* @email: Kinvy66@163.com
* @date: 2023/4/16 14:52
*/

#include "sylar/sylar.h"
#include <unistd.h>


int count = 0;
sylar::RWMutex s_mutex;

void fun1() {
    // SYLAR_LOG_INFO(g_logger) << "name: " << sylar::Thread::GetName()
    //                         << " this.name: " << sylar::Thread::GetThis()->GetName()
    //                         << " id: " << sylar::GetThreadId()
    //                         << " this.id: " << sylar::Thread::GetThis()->getId();
    // for (int i = 0; i < 100000; ++i) {
    //     sylar::RWMutex::WriteLock lock(s_mutex);
    //     ++count;
    // }
}

void fun2() {

}

void test() {
    sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();
    SYLAR_LOG_INFO(g_logger) << "thread test begin";
    YAML::Node root = YAML::LoadFile("./conf/log2.yml");
    sylar::Config::LoadFromYaml(root);
    SYLAR_LOG_INFO(g_logger) << "================================";
}

int main(int argc, char** argv) {
   test();
    return 0;
}

