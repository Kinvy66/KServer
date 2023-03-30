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
#include <map>
#include <functional>
#include <iostream>


namespace sylar {

class Logger;

/**
 * @brief 日志事件
 */
class LogEvent {
public:
    typedef std::shared_ptr<LogEvent> ptr;
    /**
     * @brief 构造函数
     */
    LogEvent(const char* file, int32_t m_line, uint32_t elapse,
             uint32_t thread_id, uint32_t fiber_id, uint64_t time);

    const char* getFile() const { return m_file; }
    uint32_t  gerLine() const { return m_line; }
    uint32_t  getElapse() const { return m_elapse; }
    uint32_t  getThreadId() const { return m_threadId; }
    uint32_t getFiberId() const { return m_fiberId; }
    uint64_t getTime() const { return m_time; }
    const std::string getContent() const { return m_ss.str(); }

    std::stringstream & getSS() { return m_ss; }


private:
    const char *m_file = nullptr;  // 文件名
    int32_t m_line = 0;            // 行号
    uint32_t m_elapse = 0;          // 程序启动到现在的毫秒数
    int32_t m_threadId = 0;        //线程id
    int32_t m_fiberId = 0;          // 协程id
    int64_t m_time;                 // 时间戳
    std::stringstream m_ss;          // 内容

};

/**
 * @brief 日志级别
 */
class LogLevel {
public:
    enum Level {
        UNKNOW = 0,
        DEBUG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4,
        FATAL = 5
    };
    static const char* ToString(LogLevel::Level level);
};

/**
 * @brief 日志格式器
 */
class LoggerFormatter {
public:
    typedef std::shared_ptr<LoggerFormatter> ptr;
    LoggerFormatter(const std::string &pattern);


    // %t  %thread_id  %m%n
    std::string  format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event);

public:
    // 格式解析
    class FormatItem {
    public:
        typedef std::shared_ptr<FormatItem> ptr;
//        FormatItem(const std::string& fmt = "") {}
        virtual ~FormatItem() {}
        virtual void format(std::ostream  &os,
                            std::shared_ptr<Logger> logger,
                            LogLevel::Level level,
                            LogEvent::ptr event) = 0;
    };

    void init();

private:
    std::string m_pattern;
    std::vector<FormatItem::ptr> m_items;
};

/**
 * @brief 日志输出地
 */
class LogAppender {
public:
    typedef std::shared_ptr<LogAppender> ptr;
    virtual ~LogAppender() = default;
    virtual void log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) = 0;

    void setFormatter(LoggerFormatter::ptr val) { m_formatter = val; }
    LoggerFormatter::ptr getFormatter() const { return m_formatter; }

protected:
    LogLevel::Level m_level = LogLevel::DEBUG;   // 针对哪些日志定义的级别
    LoggerFormatter::ptr m_formatter;
};


/**
 * @brief 日志器
 */
class Logger : public std::enable_shared_from_this<Logger> {
public:
    typedef std::shared_ptr<Logger> ptr;
    Logger(const std::string & name = "root");
    void log(LogLevel::Level level, LogEvent::ptr event);

    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);

    void addAppender(LogAppender::ptr appender);
    void delAppender(LogAppender::ptr appender);
    LogLevel::Level getLevel() const { return m_level; }
    void setLevel(LogLevel::Level val) { m_level = val; }

    const std::string& getName() const { return m_name; }

private:
    std::string  m_name;       // 日志名称
    LogLevel::Level m_level;  // 日志器级别
    std::list<LogAppender::ptr> m_appenders;  // Appender 集合
    LoggerFormatter::ptr  m_formatter;
};

// 输出到控制的appender
class StdoutAppender: public LogAppender {
public:
    typedef std::shared_ptr<StdoutAppender> ptr;
    void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override ;

private:
};

// 输出到文件的appender
class FileLogAppender: public LogAppender {
public:
    typedef std::shared_ptr<FileLogAppender> ptr;
    FileLogAppender(const std::string &filename);
    void log(Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override;

    // 重新打开文件
    bool  reopen();

private:
    std::string m_filename;
    std::ofstream m_filestream;
};

}

#endif //__SYLAR_LOG_H
