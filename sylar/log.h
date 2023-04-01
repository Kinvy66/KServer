/**
* @author: Kinvy
* @email: Kinvy66@163.com
* @date: 2023/3/30 13:11
* @file: log.h
* @description: 
*/
#ifndef __SYLAR_LOG_H
#define __SYLAR_LOG_H

#include <string>
#include <cstdint>
#include <memory>
#include <list>
#include <sstream>
#include <fstream>
#include <vector>
#include <tuple>
#include <iostream>
#include <cstdarg>
#include <map>
#include "singleton.h"

/**
 * @brief 使用流式方式将日志级别level的日志写入到logger
 */
#define SYLAR_LOG_LEVEL(logger, level) \
    if(logger->getLevel() < level)     \
        sylar::LogEventWrap(sylar::LogEvent::ptr(new sylar::LogEvent(logger, level, \
                        __FILE__, __LINE__, 0, sylar::GetThreadId(), \
                sylar::GetFiberId(), time(0)))).getSS()

/**
 * @brief 使用流式方式将日志级别debug的日志写入到logger
 */
#define SYLAR_LOG_DEBUG(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::DEBUG)

/**
 * @brief 使用流式方式将日志级别info的日志写入到logger
 */
#define SYLAR_LOG_INFO(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::INFO)

/**
 * @brief 使用流式方式将日志级别warn的日志写入到logger
 */
#define SYLAR_LOG_WARN(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::WARN)

/**
 * @brief 使用流式方式将日志级别error的日志写入到logger
 */
#define SYLAR_LOG_ERROR(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::ERROR)

/**
 * @brief 使用流式方式将日志级别fatal的日志写入到logger
 */
#define SYLAR_LOG_FATAL(logger) SYLAR_LOG_LEVEL(logger, sylar::LogLevel::FATAL)

/**
 * @brief 使用格式化方式将日志级别level的日志写入到logger
 */
#define SYLAR_LOG_FMT_LEVEL(logger, level, fmt, ...) \
    if(logger->getLevel() <= level) \
        sylar::LogEventWrap(sylar::LogEvent::ptr(new sylar::LogEvent(logger, level, \
                        __FILE__, __LINE__, 0, sylar::GetThreadId(),\
                sylar::GetFiberId(), time(0)))).getEvent()->format(fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别debug的日志写入到logger
 */
#define SYLAR_LOG_FMT_DEBUG(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::DEBUG, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别info的日志写入到logger
 */
#define SYLAR_LOG_FMT_INFO(logger, fmt, ...)  SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::INFO, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别warn的日志写入到logger
 */
#define SYLAR_LOG_FMT_WARN(logger, fmt, ...)  SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::WARN, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别error的日志写入到logger
 */
#define SYLAR_LOG_FMT_ERROR(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::ERROR, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别fatal的日志写入到logger
 */
#define SYLAR_LOG_FMT_FATAL(logger, fmt, ...) SYLAR_LOG_FMT_LEVEL(logger, sylar::LogLevel::FATAL, fmt, __VA_ARGS__)


namespace sylar {

class Logger;

/**
 * @brief 日志级别
 */
class LogLevel {
public:
    /**
     * @brief 日志级别枚举
     */
    enum Level {
        UNKNOW = 0,
        DEBUG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4,
        FATAL = 5
    };

    /**
     * @brief 将日志级别转换成文本输出
     * @param level[in] 日志级别
     * @return
     */
    static const char* ToString(LogLevel::Level level);
};

/**
 * @brief 日志事件
 */
class LogEvent {
public:
    typedef std::shared_ptr<LogEvent> ptr;
    /**
     * @brief 构造函数
     * @param logger 日志器
     * @param level 日志级别
     * @param file 文件名
     * @param m_line 文件行号
     * @param elapse 程序启动依赖的耗时（ms)
     * @param thread_id 线程id
     * @param fiber_id 协程id
     * @param time 日志时间戳 （s）
     */
    LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level,
             const char* file, int32_t m_line,
             uint32_t elapse, uint32_t thread_id,
             uint32_t fiber_id, uint64_t time);

    /**
     * @brief 返回文件名
     * @return
     */
    const char* getFile() const { return m_file; }

    /**
     * @brief 获取行号
     * @return
     */
    uint32_t  gerLine() const { return m_line; }

    /**
     * @brief 获取耗时
     * @return
     */
    uint32_t  getElapse() const { return m_elapse; }

    /**
     * @brief 获取线程id
     * @return
     */
    uint32_t  getThreadId() const { return m_threadId; }

    /**
     * @brief 获取协程id
     * @return
     */
    uint32_t getFiberId() const { return m_fiberId; }

    /**
     * @brief 获取时间
     * @return
     */
    uint64_t getTime() const { return m_time; }

    /**
     * @brief 获取日志内容
     * @return
     */
    const std::string getContent() const { return m_ss.str(); }

    /**
     * 获取日志器
     * @return
     */
    std::shared_ptr<Logger> getLogger() const { return m_logger; }

    /**
     * @brief 获取日志级别
     * @return
     */
    LogLevel::Level getLevel() const { return  m_level; }

    /**
     * @brief 获取日志内容字节流
     * @return
     */
    std::stringstream & getSS() { return m_ss; }

    /**
     * @brief 格式写入日志内容
     * @param fmt 格式
     * @param ...
     */
    void format(const char* fmt, ...);

    /**
     * @brief 格式写入日志内容
     * @param fmt
     * @param al
     */
    void format(const char* fmt, va_list al);

private:
    const char *m_file = nullptr;  // 文件名
    int32_t m_line = 0;            // 行号
    uint32_t m_elapse = 0;          // 程序启动到现在的毫秒数
    int32_t m_threadId = 0;        //线程id
    int32_t m_fiberId = 0;          // 协程id
    int64_t m_time;                 // 时间戳
    std::stringstream m_ss;          // 内容流
    std::shared_ptr<Logger> m_logger;   // 日志器
    LogLevel::Level m_level;            // 日志级别
};

/**
 * @brief 日志事件包装器
 */
class LogEventWrap {
public:
    /**
     * @brief 构造函数
     * @param e 日志事件
     */
    LogEventWrap(LogEvent::ptr e);

    /**
     * @brief 析构函数
     */
    ~LogEventWrap();

    /**
     * @brief 获取日志事件
     * @return
     */
    LogEvent::ptr getEvent() const { return m_event; }

    /**
     * @brief 获取日志内容流
     * @return
     */
    std::stringstream& getSS();

private:
    /**
     * @brief 日志事件
     */
    LogEvent::ptr  m_event;
};

/**
 * @brief 日志格式化
 */
class LoggerFormatter {
public:
    typedef std::shared_ptr<LoggerFormatter> ptr;
    /**
     * @brief 构造函数
     * @param pattern  格式模板
     * @details
     *  %m 消息体
     *  %p 日志级别
     *  %r 累计毫秒数
     *  %c 日志名称
     *  %t 线程id
     *  %n 换行
     *  %d 时间
     *  %f 文件名
     *  %l 行号
     *  %T 制表符
     *  %F 协程id
     *
     *  默认格式 "%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"
     */
    LoggerFormatter(const std::string &pattern);


    // %t  %thread_id  %m%n
    /**
     * @brief 获取格式化日志文本
     * @param logger 日志器
     * @param level 日志级别
     * @param event 日志事件
     * @return 日志格式化的字符串
     * @details
     *  在日志输出地appender被调用
     */
    std::string  format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);

public:

    /**
     *@brief 日志内容项格式化
     */
    class FormatItem {
    public:
        typedef std::shared_ptr<FormatItem> ptr;

        /**
         * @brief 析构函数
         */
        virtual ~FormatItem() = default;

        /**
         * @brief 格式化日志到流
         * @param os  日志输出流
         * @param logger 日志器
         * @param level 日志级别
         * @param event 日志事件
         */
        virtual void format(std::ostream  &os,
                            std::shared_ptr<Logger> logger,
                            LogLevel::Level level,
                            LogEvent::ptr event) = 0;
    };

    /**
     * @brief 初始化，解析日志模板
     * @details
     *  解析格式，将格式模板中的不同格式保存到m_items
     */
    void init();

private:
    std::string m_pattern;      // 日志模板
    std::vector<FormatItem::ptr> m_items;  // 日志格式解析后的格式
};

/**
 * @brief 日志输出目标
 */
class LogAppender {
public:
    typedef std::shared_ptr<LogAppender> ptr;

    /**
     * @brief 析构函数
     */
    virtual ~LogAppender() = default;

    /**
     * @brief 将日志内容流的信息写入日志输出地（控制台或文件）
     * @param logger 日志器
     * @param level 日志级别
     * @param event 日志事件
     */
    virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;

    /**
     * @brief 设置日志格式器
     * @param val
     */
    void setFormatter(LoggerFormatter::ptr val) { m_formatter = val; }

    /**
     * @brief 获取日志格式器
     * @return
     */
    LoggerFormatter::ptr getFormatter() const { return m_formatter; }

    /**
     * @brief 获取日志级别
     * @return
     */
    LogLevel::Level getLevel() const { return  m_level; }

    /**
     * @brief 设置日志级别
     * @param val
     */
    void setLevel(LogLevel::Level val) { m_level = val; }

protected:
    LogLevel::Level m_level = LogLevel::DEBUG;   // 日志级别
    LoggerFormatter::ptr m_formatter;           // 日志格式器
};


/**
 * @brief 日志器
 */
class Logger : public std::enable_shared_from_this<Logger> {
public:
    typedef std::shared_ptr<Logger> ptr;

    /**
     * @brief 构造函数
     * @param name  日志器名称
     */
    Logger(const std::string & name = "root");

    /**
     * @brief 将日志内容流输出到定义的日志输出地
     * @param level 日志级别
     * @param event 日志事件
     * @details
     *  遍历日志器定义的所有的appender,
     *  把日志的内容流输出到所有的appender中
     */
    void log(LogLevel::Level level, LogEvent::ptr event);

    /**
     * @brief 写debug级别日志
     * @param event 日志事件
     */
    void debug(LogEvent::ptr event);

    /**
     * @brief 写info级别日志
     * @param event 日志事件
     */
    void info(LogEvent::ptr event);

    /**
     * @brief 写warn级别日志
     * @param event 日志事件
     */
    void warn(LogEvent::ptr event);

    /**
     * @brief 写error级别日志
     * @param event 日志事件
     */
    void error(LogEvent::ptr event);

    /**
     * @brief 写fatal级别日志
     * @param event 日志事件
     */
    void fatal(LogEvent::ptr event);

    /**
     * @brief 添加日志目标
     * @param appender 日志目标
     */
    void addAppender(LogAppender::ptr appender);

    /**
     * @brief 删除日志目标
     * @param appender 日志目标
     */
    void delAppender(LogAppender::ptr appender);

    /**
     * @brief 获取日志级别
     * @return
     */
    LogLevel::Level getLevel() const { return m_level; }

    /**
     * @brief 设置日志级别
     * @param val
     */
    void setLevel(LogLevel::Level val) { m_level = val; }

    /**
     * @brief 获取日志名称
     * @return
     */
    const std::string& getName() const { return m_name; }

private:
    std::string  m_name;       // 日志名称
    LogLevel::Level m_level;  // 日志器级别
    std::list<LogAppender::ptr> m_appenders;  // 日志目标集合
    LoggerFormatter::ptr  m_formatter;        // 日志格式器
};

/**
 * @brief 输出到控制台的appender
 */
class StdoutAppender: public LogAppender {
public:
    typedef std::shared_ptr<StdoutAppender> ptr;
    void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override ;

private:
};

/**
 * @brief 输出到文件的appender
 */
class FileLogAppender: public LogAppender {
public:
    typedef std::shared_ptr<FileLogAppender> ptr;
    FileLogAppender(const std::string &filename);
    void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;

    /**
     * @brief 重新打开日志文件
     * @return 成功返回true
     */
    bool  reopen();

private:
    std::string m_filename;   // 文件路径
    std::ofstream m_filestream; // 文件流
};

/**
 * @brief 日志器管理类
 */
class LoggerManager {
public:
    /**
     * @brief 构造函数
     */
    LoggerManager();

    /**
     * @brief 获取日志器
     * @param name 日志器名称
     * @return
     */
    Logger::ptr getLogger(const std::string& name);

    /**
     * @brief 初始化
     */
    void init();

private:
    std::map<std::string, Logger::ptr> m_loggers;   // 日志容器
    Logger::ptr m_root;     // 主日志器
};

// 日志器管理类单例模式
typedef sylar::Singleton<LoggerManager> LoggerMgr;

}

#endif //__SYLAR_LOG_H
