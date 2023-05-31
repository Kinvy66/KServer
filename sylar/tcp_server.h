/**
* @file: tcp_server.h
* @brief:
* @author: Kinvy.Qiu
* @email: Kinvy66@163.com
* @date: 2023/5/29 
*/
#ifndef __SYLAR_TCP_SERVER_H
#define __SYLAR_TCP_SERVER_H

#include <memory>
#include <functional>
#include "iomanager.h"
#include "socket.h"
#include "address.h"
#include "noncopyable.h"

namespace sylar {

class TcpServer: public std::enable_shared_from_this<TcpServer>
                    , Noncopyable {
public:
    using ptr = std::shared_ptr<TcpServer>;

    TcpServer(IOManager* worker = IOManager::GetThis()
            , IOManager* accept_worker = IOManager::GetThis());
    virtual ~TcpServer();

    virtual bool bind(Address::ptr addr);
    virtual bool bind(const std::vector<Address::ptr>& addrs
                      , std::vector<Address::ptr>& fails);
    virtual bool start();
    virtual void stop();

    uint64_t getRecvTimeout() const { return m_recvTimeout;}
    std::string getName() const { return m_name;}
    void setReadTimeout(uint64_t v) { m_recvTimeout = v;}
    void setName(const std::string& v) { m_name = v;}

    bool isStop() const { return m_isStop;}

protected:
    virtual void handleClient(Socket::ptr client);
    virtual void startAccept(Socket::ptr sock);
private:
    std::vector<Socket::ptr> m_socks;
    IOManager* m_worker;
    IOManager* m_acceptWorker;
    uint64_t m_recvTimeout;
    std::string m_name;
    bool m_isStop;

};

}

#endif //__SYLAR_TCP_SERVER_H
