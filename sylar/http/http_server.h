/**
* @file: http_server.h
* @brief: 
* @author: Kinvy.Qiu
* @email: Kinvy66@163.com
* @date: 2023/6/2 
*/
#ifndef __SYLAR_HTTP_SERVER_H
#define __SYLAR_HTTP_SERVER_H

#include "sylar/tcp_server.h"
#include "http_session.h"
#include "servlet.h"

namespace sylar {
namespace http {

class HttpServer : public TcpServer {
public:
    using ptr = std::shared_ptr<HttpServer>;
    HttpServer(bool keepalive = false
            , sylar::IOManager* worker = sylar::IOManager::GetThis()
            ,sylar::IOManager* accept_worker = sylar::IOManager::GetThis());

    ServletDispatch::ptr getServletDispatch() const { return m_dispatch;}
    void setDispatch(ServletDispatch::ptr v) { m_dispatch = v;}

protected:
    virtual void handleClient(Socket::ptr client) override;
private:
    bool m_isKeepalive;
    ServletDispatch::ptr m_dispatch;
};

}
}


#endif //__SYLAR_HTTP_SERVER_H
