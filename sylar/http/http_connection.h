/**
* @file: http_connection.h
* @brief: 
* @author: Kinvy.Qiu
* @email: Kinvy66@163.com
* @date: 2023/6/5 
*/
#ifndef __SYLAR_HTTP_CONNECTION_H
#define __SYLAR_HTTP_CONNECTION_H


#include "sylar/socket_stream.h"
#include "http.h"

namespace sylar {
namespace http {
class HttpConnection : public SocketStream {
public:
    using ptr = std::shared_ptr<HttpConnection>;
    HttpConnection(Socket::ptr sock, bool owner = true);
    HttpResponse::ptr recvResponse();
    int sendRequest(HttpRequest::ptr rsq);
};

}
}
#endif //__SYLAR_HTTP_CONNECTION_H
