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

### Log4J

```
Logger(定义日志类别)
 |
 |-----Formatter(日志格式)
 |
Appender(日志输出地方)

```


## 配置系统

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
      - type: (StdoutAppender, FileLogAppender)
        level: (debug,info,warn,fatal)
        file: /log/xxx.log
```






## 协程库封装





## socket函数库



## htttp 协议开发





## 分布协议





## 推荐系统





## 聊天软件





