# Hook 模块
hook实际上就是对系统调用API进行一次封装，将其封装成一个与原始的系统调用API同名的接口，应用在调用这个接口时，会先执行封装中的操作，再执行原始的系统调用API。


## hook 实现原理
hook的实现是基于动态链接库加载的原理，  
在使用多个动态库时，两个动态库之间有可能存在相同名称的函数，由于动态链接器在程序加载时会对外部引用进行重定位，这样会出现只有第一个函数生效，即所有对该函数的调用都将指向第一个加载的动态库的同名函数中  
sylar系统是被编译成一个动态库，运行时优先调用，hook模块种有和系统调用同名的函数，自然就会调用已被hook的函数


关于hook的另一个讨论点是如何找回已经被全局符号介入机制覆盖的系统调用接口，这个功能非常实用，因为大部分情况下，系统调用提供的功能都是无可替代的，我们虽然可以用hook的方式将其替换成自己的实现，但是最终要实现的功能，还是得由原始的系统调用接口来完成。

以malloc和free为例，假如我们要hook标准库提供的malloc和free接口，以跟踪每次分配和释放的内存地址，判断有无内存泄漏问题，那么具体的实现方式应该是，先调用自定义的malloc和free实现，在分配和释放内存之前，记录下内存地址，然后再调用标准库里的malloc和free，以真正实现内存申请和释放。

上面的过程涉及到了查找后加载的动态库里被覆盖的符号地址问题。首先，这个操作本身就具有合理性，因为程序运行时，依赖的动态库无论是先加载还是后加载，最终都会被加载到程序的进程空间中，也就是说，那些因为加载顺序靠后而被覆盖的符号，它们只是被“雪藏”了而已，实际还是存在于程序的进程空间中的，通过一定的办法，可以把它们再找回来。在Linux中，这个方法就是dslym，它的函数原型如下：

```c++
#define _GNU_SOURCE
#include <dlfcn.h>
 
void *dlsym(void *handle, const char *symbol);
```

关于`dlsym`的使用可参考`man 3 dlsym`，在链接时需要指定 `-ldl` 参数。使用`dlsym`找回被覆盖的符号时，第一个参数固定为 `RTLD_NEXT`，第二个参数为符号的名称
下面通过dlsym来实现上面的内存跟踪功能：
```c++
#define _GNU_SOURCE
#include <dlfcn.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
 
typedef void* (*malloc_func_t)(size_t size);
typedef void (*free_func_t)(void *ptr);
 
// 这两个指针用于保存libc中的malloc和free的地址
malloc_func_t sys_malloc = NULL;
free_func_t sys_free = NULL;
 
// 重定义malloc和free，在这里重定义会导致libc中的同名符号被覆盖
// 这里不能调用带缓冲的printf接口，否则会出段错误
void *malloc(size_t size) {
    // 先调用标准库里的malloc申请内存，再记录内存分配信息，这里只是简单地将内存地址和长度打印出来
    void *ptr = sys_malloc(size);
    fprintf(stderr, "malloc: ptr=%p, length=%ld\n", ptr, size);
    return ptr;
}
void free(void *ptr) {
    // 打印内存释放信息，再调用标准库里的free释放内存
    fprintf(stderr, "free: ptr=%p\n", ptr);
    sys_free(ptr);
}
 
int main() {
    // 通过dlsym找到标准库中的malloc和free的符号地址
    sys_malloc = dlsym(RTLD_NEXT, "malloc");
    assert(dlerror() == NULL);
    sys_free = dlsym(RTLD_NEXT, "free");
    assert(dlerror() == NULL);
 
    char *ptrs[5];
 
    for(int i = 0; i < 5; i++) {
        ptrs[i] = malloc(100 + i);
        memset(ptrs[i], 0, 100 + i);
    }
     
    for(int i = 0; i < 5; i++) {
        free(ptrs[i]);
    }
    return 0;
}
```


编译运行以上代码，效果如下：

```shell
# gcc hook_malloc.c -ldl
# ./a.out
malloc: ptr=0x55775fa8e2a0, length=100
malloc: ptr=0x55775fa8e310, length=101
malloc: ptr=0x55775fa8e380, length=102
malloc: ptr=0x55775fa8e3f0, length=103
malloc: ptr=0x55775fa8e460, length=104
free: ptr=0x55775fa8e2a0
free: ptr=0x55775fa8e310
free: ptr=0x55775fa8e380
free: ptr=0x55775fa8e3f0
free: ptr=0x55775fa8e460
```



使用 `ldd` 工具可以查看加载的动态库
```shell
$ ldd ./test_hook                                                       
        linux-vdso.so.1 (0x00007ffc4bdc0000)                                         
        libsylar.so => /mnt/e/Rep/sylar/lib/libsylar.so (0x00007f25fe778000)         
        libdl.so.2 => /lib/x86_64-linux-gnu/libdl.so.2 (0x00007f25fe75c000)          
        libpthread.so.0 => /lib/x86_64-linux-gnu/libpthread.so.0 (0x00007f25fe739000)
        libstdc++.so.6 => /lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007f25fe557000)
        libgcc_s.so.1 => /lib/x86_64-linux-gnu/libgcc_s.so.1 (0x00007f25fe53c000)
        libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007f25fe34a000)
        /lib64/ld-linux-x86-64.so.2 (0x00007f25feac8000)
        libm.so.6 => /lib/x86_64-linux-gnu/libm.so.6 (0x00007f25fe1f9000)

```
`libsylar.so` 是在 `libc.so.6` 之前加载的，所有和系统API同名的调用优于使用被hook的函数



## 具体实现
下面以 `sleep` 函数描述sylar对hook的具体实现

```c++
// hook.h
extern "C" {
// 声明以sleep_f的外部函数指针变量，其函数类型和sleep一样
typedef unsigned int (*sleep_fun)(unsigned int seconds);
extern sleep_fun sleep_f;
}
```

在 `hook_init()` 中给 `sleep_f` 赋值，其值是系统对应api动态库的函数地址

```c++
// hook.cpp
void hook_init() {
static bool is_inited = false;
    if (is_inited) {
    return;
    }
    sleep_f = (sleep_fun) dlsym(((void*) -1l), "sleep");
}
struct _HookIniter {
    _HookIniter() {
        hook_init();
       // ...
    }
};
// 静态变量，main函数之前初始化
static _HookIniter s_hook_initer;

// hook对=对应函数的实现
unsigned int sleep(unsigned int seconds) {
    if (!sylar::t_hook_enable) {
        return sleep_f(seconds);  // 没有开启hook，直接调用系统的api
    }
    
    // 开启hook
    sylar::Fiber::ptr fiber = sylar::Fiber::GetThis();
    sylar::IOManager* iom = sylar::IOManager::GetThis();
    iom->addTimer(seconds * 1000, std::bind((void (sylar::Scheduler::*)
    (sylar::Fiber::ptr, int thread))&sylar::IOManager::schedule, iom, fiber, -1));
    sylar::Fiber::YieldToHold();
    return 0;
}
```
`sleep` 的hook实现是添加一个定时事件，然后退出当前的协程，由调度器调度其他任务，定时时间到了再回到这个任务  
系统提供的 `sleep` 是线程阻塞等待。











