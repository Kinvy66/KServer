/**
* @file test_daemon.cpp
* @brief 
* @author Kinvy
* @email Kinvy66@163.com
* @date 2023-09-25
*/

#include "sylar/daemon.h"
#include "sylar/iomanager.h"
#include "sylar/log.h"

static sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();

sylar::Timer::ptr timer;
int server_main(int argc, char** argv) {
    SYLAR_LOG_INFO(g_logger) << sylar::ProcessInfoMgr::GetInstance()->toString();
    sylar::IOManager iom(1);
    timer = iom.addTimer(1000, [](){
        SYLAR_LOG_INFO(g_logger) << "onTimer";
        static int count = 0;
        if (++count > 10) {
            timer->cancel();
        }
    }, true);
    return 0;
}

int main(int argc, char** argv) {
    return sylar::start_daemon(argc, argv, server_main, argc != 1);
}