/**
* @file: test_scheduler.cpp
* @brief: 
* @author: Kinvy.Qiu
* @email: Kinvy66@163.com
* @date: 2023/4/21 
*/

#include "sylar/sylar.h"

static sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();

int main(int argc, char** argv) {
    sylar::Scheduler sc;
    sc.start();
    sc.stop();
    return 0;
}