/**
* @file http_server.h
* @brief HTTP服务器封装
* @author Kinvy
* @email Kinvy66@163.com
* @date 2023-6-2
*/
#ifndef __SYLAR_HTTP_SERVER_H
#define __SYLAR_HTTP_SERVER_H

#include "sylar/tcp_server.h"
#include "http_session.h"
#include "servlet.h"

namespace sylar {
namespace http {

/**
 * @brief HTTP服务器类
 */
class HttpServer : public TcpServer {
public:
    using ptr = std::shared_ptr<HttpServer>;

    /**
     * @brief 构造函数
     * @param[in] keepalive 是否长连接
     * @param[in] worker 工作调度器
     * @param[in] accept_worker 接收连接调度器
     */
    HttpServer(bool keepalive = false
            , sylar::IOManager* worker = sylar::IOManager::GetThis()
            ,sylar::IOManager* accept_worker = sylar::IOManager::GetThis());

    /**
     * @brief 获取ServletDispatch
     */
    ServletDispatch::ptr getServletDispatch() const { return m_dispatch;}

    /**
     * @brief 设置ServletDispatch
     */
    void setDispatch(ServletDispatch::ptr v) { m_dispatch = v;}

protected:
    virtual void handleClient(Socket::ptr client) override;
private:
    /// 是否支持长连接
    bool m_isKeepalive;
    /// Servlet分发器
    ServletDispatch::ptr m_dispatch;
};

}
}

#endif //__SYLAR_HTTP_SERVER_H
