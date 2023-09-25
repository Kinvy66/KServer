/**
* @file daemon.h
* @brief 
* @author Kinvy
* @email Kinvy66@163.com
* @date 2023-09-25
*/
#ifndef __SYLAR_DAEMON_H
#define __SYLAR_DAEMON_H

#include <functional>
#include <unistd.h>
#include "sylar/singleton.h"

namespace sylar {

struct ProcessInfo {
    /// 父进程id
    pid_t parent_id = 0;
    /// 主进程id
    pid_t main_id = 0;
    /// 父进程启动时间
    uint64_t parent_start_time = 0;
    /// 主进程启动时间
    uint64_t main_start_time = 0;
    /// 主进程重启的次数
    uint32_t restart_count = 0;

    std::string toString() const;
};

typedef sylar::Singleton<ProcessInfo> ProcessInfoMgr;

/**
 * @brief 启动程序可以选择用守护进程的方式
 * @param argc 参数个数
 * @param argv 参数值数组
 * @param main_cb 启动函数
 * @param is_daemon 是否守护进程的方式
 * @return 返回程序的执行结果
 */
int start_daemon(int argc, char** argv,
                 std::function< int(int argc, char** argv)> main_cb
                 , bool is_daemon);
}


#endif //__SYLAR_DAEMON_H
