# Linux 网络编程


## 1. linux 网络编程基础api

### 1.1 字节序

- 小端(主机字节序)： 低位在低地址，高位在高地址
- 大端(网络字节序)： 低位在高地址，高位在低地址


linux提供的字节序转换api:
```c
#include <netinet/in.h>
unsigned long int htonl(unsigned long int hostlong);
unsigned short int htons(unsigned short int hostshort);
unsigned long int ntohl(unsigned long int netlong);
unsigned short int ntohs(unsigned short int netshort);
```
函数名含义： `htonl` : host to network long, `s`表示 short


### 1.2 socket 地址

**通用socket地址**的结构体 `sockaddr`
```c
#include <bits.socket.h>
struct sockaddr {
    sa_family_t sa_family;
    char sa_data[14];
};
```
`sa_family_t` 地址族类型，与协议族(domain)对应，对应的具有相同的值

| 协议族      | 地址族      | 描述       |
|----------|----------|----------|
| PF_UNIX  | AF_UNIX  | UNIX 本地域 |
| PF_INET  | AF_INET  | IPv4     |
| PF_INET6 | AF_INET6 | IPv6     |


`sa_data` 用于存放 socket地址值



**专用socket地址**


### 1.3 IP地址转换函数



### 1.4 socket 函数

```c
// 创建一个套接字
int socket(int domain, int type, int protocol);
```
参数:
- `domain`: 使用的地址族协议
  - `AF_INET`: 使用IPv4格式的ip地址
  - `AF_INET6`: 使用IPv4格式的ip地址
- `type`:
  - `SOCK_STREAM`: 使用流式的传输协议 
  - `SOCK_DGRAM`: 使用报式(报文)的传输协议
- `protocol`: 一般写0即可, 使用默认的协议 
  - `SOCK_STREAM`: 流式传输默认使用的是tcp 
  - `SOCK_DGRAM`: 报式传输默认使用的udp
- 返回值:
  - 成功: 可用于套接字通信的文件描述符 
  - 失败: -1



```c
// 将文件描述符和本地的IP与端口进行绑定   
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
```

参数:
- `sockfd`: 监听的文件描述符, 通过socket()调用得到的返回值
- `addr`: 传入参数, 要绑定的IP和端口信息需要初始化到这个结构体中，IP和端口要转换为网络字节序
- `addrlen`: 参数addr指向的内存大小, sizeof(struct sockaddr)

返回值：成功返回0，失败返回-1


```c
// 给监听的套接字设置监听
int listen(int sockfd, int backlog);
```


