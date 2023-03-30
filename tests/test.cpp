/**
* @author: Kinvy
* @email: Kinvy66@163.com
* @date: 2023/3/30 19:34
* @file: test.cpp
* @description: 
*/
#include <iostream>
#include "../sylar/log.h"

int main(int argc, char** argv) {

    sylar::Logger::ptr logger(new sylar::Logger);
    logger->addAppender(sylar::LogAppender::ptr(new sylar::StdoutAppender));

    sylar::LogEvent::ptr event(new sylar::LogEvent(__FILE__, __LINE__, 0, 1, 2, time(0)));

    logger->log(sylar::LogLevel::DEBUG, event);

    std::cout << "Hello " << std::endl;

    return 0;
}