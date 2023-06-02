/**
* @file: http_session.h
* @brief: 
* @author: Kinvy.Qiu
* @email: Kinvy66@163.com
* @date: 2023/5/31 
*/
#ifndef __SYLAR_HTTP_SESSION_H
#define __SYLAR_HTTP_SESSION_H

#include "sylar/socket_stream.h"
#include "http.h"

namespace sylar {
namespace http {
class HttpSession : public SocketStream {
public:
    using ptr = std::shared_ptr<HttpSession>;
    HttpSession(Socket::ptr sock, bool owner = true);
    HttpRequest::ptr recvRequest();
    int sendResponse(HttpResponse::ptr rsp);


};

}
}

#endif //__SYLAR_HTTP_SESSION_H
