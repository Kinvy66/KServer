/**
* @file env.h
* @brief 
* @author Kinvy
* @email Kinvy66@163.com
* @date 2023-09-25
*/
#ifndef __SYLAR_ENV_H
#define __SYLAR_ENV_H

#include "sylar/singleton.h"
#include "sylar/thread.h"
#include <map>
#include <vector>

namespace sylar {

class Env {
public:
    typedef RWMutex RWMutexType;
    bool init(int argc, char** argv);

    void add(const std::string& key, const std::string& val);
    bool has(const std::string& key);
    void del(const std::string& key);
    std::string get(const std::string& key, const std::string& default_val = "");

    void addHelp(const std::string& key, const std::string& desc);
    void removeHelp(const std::string& key);
    void printHelp();

    const std::string& getExe() const { return m_exe; }
    const std::string& getCwd() const { return m_cwd; }

    bool setEnv(const std::string& key, const std::string& val);
    std::string getEnv(const std::string& key, const std::string& default_val="");

    std::string getAbsolutePath(const std::string& path) const;
private:
    RWMutexType m_mutex;
    std::map<std::string, std::string> m_args;
    std::vector<std::pair<std::string, std::string>> m_helps;
    std::string m_program;
    std::string m_exe;
    std::string m_cwd;
};

typedef sylar::Singleton<Env> EnvMgr;

}

#endif //__SYLAR_ENV_H
