# C++ 服务器框架开发

参考： https://github.com/sylar-yin/sylar

## 环境

- WSL2-- Ubuntu20 (GCC 9.4)
- Clion 2023
- CMake



## 项目环境

bin ----- 二进制

build ---- 中间文件路径

cmake----cmake函数文件

CMakeList.txt ---- cmake的定义文件

lib  ---- 库的输出路径

Makefile

sylar ---- 源码路径

tests----测试代码




## 日志系统

日志系统设计类图

![log-uml](https://kinvy-images.oss-cn-beijing.aliyuncs.com/Images/diagram-6304072420879448753.png)

使用
```c++
    sylar::Logger::ptr logger(new sylar::Logger);
    logger->addAppender(sylar::LogAppender::ptr(new sylar::StdoutLogAppender));

    sylar::FileLogAppender::ptr file_appender(new sylar::FileLogAppender("./log.txt"));

    sylar::LoggerFormatter::ptr fmt(new sylar::LoggerFormatter("%d%T%m%n"));
    file_appender->setFormatter(fmt);
    file_appender->setLevel(sylar::LogLevel::ERROR);

//    logger->addAppender(file_appender);
//    sylar::LogEvent::ptr event(new sylar::LogEvent(logger,
//                                                   sylar::LogLevel::DEBUG,
//                                                   __FILE__, __LINE__,
//                                                   0, sylar::GetThreadId(),
//                                                   sylar::GetFiberId(), time(0)));
//    event->getSS() << "Hello ";
//    logger->log(sylar::LogLevel::DEBUG, event);

    SYLAR_LOG_INFO(logger) << "test info";
    SYLAR_LOG_ERROR(logger) << "test error";

    SYLAR_LOG_FMT_ERROR(logger, "test fmt error %s", "aa");

    auto  l = sylar::LoggerMgr::GetInstance()->getLogger("xx");
    SYLAR_LOG_INFO(l) << "xxx";
```

### Log4J

```
Logger(定义日志类别)
 |
 |-----Formatter(日志格式)
 |
Appender(日志输出地方)

```
Logger 默认构造函数生成的logger
```c++
name: root
level: DEBUG
format: %d{%Y-%m-%d %H:%M:%S}%T%t%T%F%T[%p]%T[%c]%T%f:%l%T%m%n
```



## 配置系统

![config-uml](https://kinvy-images.oss-cn-beijing.aliyuncs.com/Images/diagram-17344954375024489049.png)

使用的第三方库
- [yaml-cpp](https://github.com/jbeder/yaml-cpp)
- [boost](https://www.boost.org/)

```c++
class ConfigVarBase {
    // 配置属性的基本类，抽象类
    // 子类需要实现字符串和属性值之间的转换
};

template<typename F, typename T>
class LexicalCast { 
    // 用于转换的函数对象
    // 只定义了调用操作符重载
};

template<typename T, typename FromStr = LexicalCast<std::string, T>,
        typename ToStr = LexicalCast<T, std::string> >
class ConfigVar : public ConfigVarBase {
    // 具体的配置属性
    // 调用可函数对象 FromStr/ToStr 实现类型T和string之间的转换
    // 不同类型的转换需要偏特化LexicalCast
};

class Config { 
    // 配置管理类 
};

```



配置系统的原则，约定优于配置：

```c++
template<T, FromStr, ToStr>
class ConfigVar;

template<F, T>
LexicalCast;

// 容器偏特化， 目前支持 vector
// list, set, map, unordered_set, unordered_map
// map/unordered_set 支持 key = std::string
// Config::Lookup(key), key相同
// 类型不同的，不会有报错，这个需要处理一下
```

自定义类型需要实现 sylar::LexicalCast偏特化
驶行定义后，就可以支持Config解析自定义类型，自定义类型可以和常规stl容器一起使用

配置的事件机制
当一个配置项发生修改的时候，可以反向通知对应的代码，回调
该功实现使用了观察者模式？

### 日志系统整合配置系统

```yaml
logs:
  - name: root
    level: (debug,info,warn,fatal)
    formatter: '%d%T%p%T%t%m%n'
    appender:
      - type: (StdoutLogAppender, FileLogAppender)
        level: (debug,info,warn,fatal)
        file: /log/xxx.log
```

```c++
sylar::Logger g_logger = sylar::LoggerMgr::GetInstance()->getLogger(name);
SYLAR_LOG_INFO(g_logger) << "xxxx log";
```

```c++
static Logger::ptr g_log = SYLAR_LOG_NAME("system");
// m_root, m_system-> m_root 当 logger 的appenders为空， 使用 root 写logger
```

```c++

```




## 协程库封装





## socket函数库



## htttp 协议开发





## 分布协议





## 推荐系统





## 聊天软件





