/**
* @file: test_http.cpp
* @brief: 
* @author: Kinvy.Qiu
* @email: Kinvy66@163.com
* @date: 2023/5/27 
*/

#include "sylar/http.h"
#include "sylar/log.h"

void test_request() {
    sylar::http::HttpRequest::ptr req(new sylar::http::HttpRequest);
    req->setHeader("host", "www.kinvy.cn");
    req->setBody("hello kinvy");
    req->dump(std::cout) << std::endl;
}

void test_response() {
    sylar::http::HttpResponse::ptr  rsp(new sylar::http::HttpResponse);
    rsp->setHeader("X-X", "kinvy");
    rsp->setBody("hello kinvy");
    rsp->setStatus((sylar::http::HttpStatus)400);
    rsp->setClose(false);
    rsp->dump(std::cout) << std::endl;
}

int main(int argc, char** argv) {
    test_request();
    test_response();
    return 0;
}

