/**
* @file test_env.cpp
* @brief 
* @author Kinvy
* @email Kinvy66@163.com
* @date 2023-09-25
*/

#include "sylar/env.h"
#include <iostream>

int main(int argc, char** argv) {
    sylar::EnvMgr::GetInstance()->addHelp("s", "start with the terminal");
    sylar::EnvMgr::GetInstance()->addHelp("d", "run as daemon");
    sylar::EnvMgr::GetInstance()->addHelp("p", "print help");
    if (!sylar::EnvMgr::GetInstance()->init(argc, argv)) {
        sylar::EnvMgr::GetInstance()->printHelp();
        return 0;
    }
    std::cout << "exe=" << sylar::EnvMgr::GetInstance()->getExe() << std::endl;
    std::cout << "exe=" << sylar::EnvMgr::GetInstance()->getCwd() << std::endl;
    std::cout << "path=" << sylar::EnvMgr::GetInstance()->getEnv("PATH", "xxx") << std::endl;

    if (sylar::EnvMgr::GetInstance()->has("p")) {
        sylar::EnvMgr::GetInstance()->printHelp();
    }
    return 0;
}
