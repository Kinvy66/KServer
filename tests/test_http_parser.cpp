/**
* @file: test_http_parser.cpp
* @brief: http 协议解析测试
* @author: Kinvy.Qiu
* @email: Kinvy66@163.com
* @date: 2023/5/28 
*/

#include "sylar/http/http_parser.h"
#include "sylar/log.h"

static sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();

const char test_request_data[] = "POST / HTTP/1.1\r\n"
                                 "Host: www.kinvy.cn\r\n"
                                 "Content-Length: 10\r\n\r\n"
                                 "1234567890";

void test_request() {
    sylar::http::HttpRequestParser parser;
    std::string tmp = test_request_data;
    size_t s = parser.execute(&tmp[0], tmp.size());
    SYLAR_LOG_INFO(g_logger) << "execute rt=" << s
        << " has_error=" << parser.hasError()
        << " is_finished=" << parser.isFinished()
        << " total=" << tmp.size()
        << " content-length=" << parser.getContentLength();
    tmp.resize(tmp.size() - s);
    SYLAR_LOG_INFO(g_logger) << parser.getData()->toSting();
}

int main(int argc, char** argv) {
    test_request();
    return 0;
}