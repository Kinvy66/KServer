/**
* @file: socket_stream.h
* @brief: 
* @author: Kinvy.Qiu
* @email: Kinvy66@163.com
* @date: 2023/5/31 
*/
#ifndef __SYLAR_SOCKET_STREAM_H
#define __SYLAR_SOCKET_STREAM_H

#include "stream.h"
#include "socket.h"

namespace sylar {

class SocketStream : public Stream {
public:
    using ptr = std::shared_ptr<SocketStream>;
    SocketStream(Socket::ptr sock, bool owner = true);
    ~SocketStream();

    virtual int read(void* buffer, size_t length) override;
    virtual int read(ByteArray::ptr ba, size_t length) override;
    virtual int write(const void* buffer, size_t length) override;
    virtual int write(ByteArray::ptr ba, size_t length) override;
    virtual void close() override;

    Socket::ptr getSocket() const { return m_socket; }
    bool isConnected() const;

protected:
    Socket::ptr m_socket;
    bool m_owner;
private:


};

}


#endif //SYLAR_SOCKET_STREAM_H
