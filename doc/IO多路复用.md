# IO多路复用
在有多个阻塞IO的应用场景种，我们可以使用多进程，多线程实现并发，但是这两者都会导致程序更复杂。  
单进程实现非阻塞IO的方法，使用轮询，但是浪费CPU。

IO多路复用基本原理，由内核检测多个文件描述符的读写缓存区的状态

三种IO多路复用的技术：select,poll,epoll, select支持多平台，poll,epoll只支持linux  
效率：select,poll底层使用线性表，epoll使用的是红黑树

## select

`select` 函数原型
```c++
#include <sys/select.h>
struct timeval {
    time_t      tv_sec;         /* seconds */
    suseconds_t tv_usec;        /* microseconds */
};

int select(int nfds, fd_set *readfds, fd_set *writefds,
           fd_set *exceptfds, struct timeval * timeout);
```
函数参数：
- `nfds`：委托内核检测的这三个集合中最大的文件描述符 + 1, 
  - 内核需要线性遍历这些集合中的文件描述符，这个值是循环结束的条件
  - 在Window中这个参数是无效的，指定为-1即可
- `readfds`：文件描述符的集合, 内核只检测这个集合中文件描述符对应的读缓冲区
  - 传入传出参数，读集合一般情况下都是需要检测的，这样才知道通过哪个文件描述符接收数据
- `writefds`：文件描述符的集合, 内核只检测这个集合中文件描述符对应的写缓冲区
  - 传入传出参数，如果不需要使用这个参数可以指定为NULL
- `exceptfds`：文件描述符的集合, 内核检测集合中文件描述符是否有异常状态
  - 传入传出参数，如果不需要使用这个参数可以指定为NULL
- `timeout`：超时时长，用来强制解除select()函数的阻塞的 
  - NULL：函数检测不到就绪的文件描述符会一直阻塞。
  - 等待固定时长（秒）：函数检测不到就绪的文件描述符，在指定时长之后强制解除阻塞，函数返回0
  - 不等待：函数不会阻塞，直接将该参数对应的结构体初始化为0即可。

函数返回值：
- 大于0：成功，返回集合中已就绪的文件描述符的总个数
- 等于-1：函数调用失败
- 等于0：超时，没有检测到就绪的文件描述符

另外初始化fd_set类型的参数还需要使用相关的一些列操作函数，具体如下：
```c
// 将文件描述符fd从set集合中删除 == 将fd对应的标志位设置为0        
void FD_CLR(int fd, fd_set *set);
// 判断文件描述符fd是否在set集合中 == 读一下fd对应的标志位到底是0还是1
int  FD_ISSET(int fd, fd_set *set);
// 将文件描述符fd添加到set集合中 == 将fd对应的标志位设置为1
void FD_SET(int fd, fd_set *set);
// 将set集合中, 所有文件文件描述符对应的标志位设置为0, 集合中没有添加任何文件描述符
void FD_ZERO(fd_set *set);
```


## poll

poll 与 select类似

```c++
#include <poll.h>
// 每个委托poll检测的fd都对应这样一个结构体
struct pollfd {
    int   fd;         /* 委托内核检测的文件描述符 */
    short events;     /* 委托内核检测文件描述符的什么事件 */
    short revents;    /* 文件描述符实际发生的事件 -> 传出 */
};

struct pollfd myfd[100];
int poll(struct pollfd *fds, nfds_t nfds, int timeout);
```

函数参数：

- `fds`: 这是一个struct pollfd类型的数组, 里边存储了待检测的文件描述符的信息，这个数组中有三个成员：
  - `fd`：委托内核检测的文件描述符
  - `events`：委托内核检测的fd事件（输入、输出、错误），每一个事件有多个取值 
  - `revents`：这是一个传出参数，数据由内核写入，存储内核检测之后的结果
- `nfds`: 这是第一个参数数组中最后一个有效元素的下标 + 1（也可以指定参数1数组的元素总个数）
- `timeout`: 指定poll函数的阻塞时长
  - -1：一直阻塞，直到检测的集合中有就绪的文件描述符（有事件产生）解除阻塞 
  - 0：不阻塞，不管检测集合中有没有已就绪的文件描述符，函数马上返回 
  - 大于0：阻塞指定的毫秒（ms）数之后，解除阻塞

函数返回值：
- 失败： 返回-1
- 成功：返回一个大于0的整数，表示检测的集合中已就绪的文件描述符的总个数


## epoll

### epoll 的操作函数
```c++
#include <sys/epoll.h>
// 创建epoll实例，通过一棵红黑树管理待检测集合
int epoll_create(int size);
// 管理红黑树上的文件描述符(添加、修改、删除)
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
// 检测epoll树中是否有就绪的文件描述符
int epoll_wait(int epfd, struct epoll_event * events, int maxevents, int timeout);
```

### 操作步骤




### 工作模式




