/**
* @file: test_http_connection.cpp
* @brief: 
* @author: Kinvy.Qiu
* @email: Kinvy66@163.com
* @date: 2023/6/5 
*/

#include <iostream>
#include "sylar/http/http_connection.h"
#include "sylar/log.h"
#include "sylar/iomanager.h"

static sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();

void run() {
    sylar::Address::ptr addr = sylar::Address::LookupAnyIPAddress("www.sylar.top:80");
    // sylar::Address::ptr addr = sylar::Address::LookupAnyIPAddress("www.kinvy.cn:80");
    if (!addr) {
        SYLAR_LOG_INFO(g_logger) << "get addr error";
        return;
    }

    sylar::Socket::ptr  sock = sylar::Socket::CreateTCP(addr);
    bool rt = sock->connect(addr);
    if (!rt) {
        SYLAR_LOG_INFO(g_logger) << "connect " << *addr << " failed";
        return;
    }
    sylar::http::HttpConnection::ptr conn(new sylar::http::HttpConnection(sock));
    sylar::http::HttpRequest::ptr req(new sylar::http::HttpRequest);
    req->setHeader("host", "www.sylar.top");
    // req->setHeader("host", "www.kinvy.cn");
    SYLAR_LOG_INFO(g_logger) << "req: " << std::endl
        << *req;
    conn->sendRequest(req);
    auto rsp = conn->recvResponse();

    if (!rsp) {
        SYLAR_LOG_INFO(g_logger) << "recv response error";
        return;
    }
    SYLAR_LOG_INFO(g_logger) << "rsp:" << std::endl
        << *rsp;
}

int main(int argc, char** argv) {
    sylar::IOManager iom(2);
    iom.schedule(run);
    return 0;
}