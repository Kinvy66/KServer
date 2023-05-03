/**
* @file: test_iomanager.cpp
* @brief: iomanager 测试
* @author: Kinvy.Qiu
* @email: Kinvy66@163.com
* @date: 2023/5/3 
*/

#include "sylar/sylar.h"
#include "sylar/iomanager.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <unistd.h>

sylar::Logger::ptr g_logger = SYLAR_LOG_ROOT();

int sock = 0;

void test_fiber() {
    SYLAR_LOG_INFO(g_logger) << "test_fiber";
}

void test1() {
    sylar::IOManager iom;
    iom.schedule(&test_fiber);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    fcntl(sock, F_SETFL, O_NONBLOCK);

    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port  = htons(80);
    inet_pton(AF_INET, "180.101.50.188", &addr.sin_addr.s_addr);

    // iom.addEvent(sock, sylar::IOManager::WRITE, [] {
    //     SYLAR_LOG_INFO(g_logger) << "connected";
    // });

    if (!connect(sock, (const sockaddr*)&addr, sizeof(addr))) {
    } else if (errno == EINPROGRESS) {
        SYLAR_LOG_INFO(g_logger) << "add event errno=" << errno << " " << strerror(errno);
        sylar::IOManager::GetThis()->addEvent(sock, sylar::IOManager::READ, []() {
            SYLAR_LOG_INFO(g_logger) << "read callback";
        });
        sylar::IOManager::GetThis()->addEvent(sock, sylar::IOManager::WRITE, []() {
            SYLAR_LOG_INFO(g_logger) << "write callback";
            // close(sock);
            sylar::IOManager::GetThis()->cancelEvent(sock, sylar::IOManager::READ);
        });
    } else {
        SYLAR_LOG_INFO(g_logger) << "else " << errno << " " << strerror(errno);
    }

}
int main(int argc, char** argv) {
    test1();
    return 0;
}