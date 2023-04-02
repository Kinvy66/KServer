/**
* @author: Kinvy
* @email: Kinvy66@163.com
* @date: 2023/3/30 19:34
* @file: test.cpp
* @description: 
*/
#include <iostream>
#include "sylar/log.h"
#include "sylar/util.h"

void test01() {
    sylar::Logger::ptr logger(new sylar::Logger);
    logger->addAppender(sylar::LogAppender::ptr(new sylar::StdoutAppender));
    sylar::LogEvent::ptr event(new sylar::LogEvent(logger,
                                                   sylar::LogLevel::INFO,
                                                   __FILE__, __LINE__,
                                                   0, sylar::GetThreadId(),
                                                   sylar::GetFiberId(), time(0)));
    event->getSS() << "Hello";
    logger->log(sylar::LogLevel::DEBUG, event);
}

void test02() {
    sylar::Logger::ptr logger(new sylar::Logger);
    logger->addAppender(sylar::LogAppender::ptr(new sylar::StdoutAppender));

    sylar::FileLogAppender::ptr file_appender(new sylar::FileLogAppender("./log.txt"));

    sylar::LoggerFormatter::ptr fmt(new sylar::LoggerFormatter("%d%T%m%n"));
    file_appender->setFormatter(fmt);
    file_appender->setLevel(sylar::LogLevel::ERROR);

//    logger->addAppender(file_appender);
//    sylar::LogEvent::ptr event(new sylar::LogEvent(logger,
//                                                   sylar::LogLevel::DEBUG,
//                                                   __FILE__, __LINE__,
//                                                   0, sylar::GetThreadId(),
//                                                   sylar::GetFiberId(), time(0)));
//    event->getSS() << "Hello ";
//    logger->log(sylar::LogLevel::DEBUG, event);

    SYLAR_LOG_INFO(logger) << "test info";
    SYLAR_LOG_ERROR(logger) << "test error";

    SYLAR_LOG_FMT_ERROR(logger, "test fmt error %s", "aa");

    auto  l = sylar::LoggerMgr::GetInstance()->getLogger("xx");
    SYLAR_LOG_INFO(l) << "xxx";
}


int main(int argc, char** argv) {

    test01();

    return 0;
}