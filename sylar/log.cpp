/**
* @author: Kinvy
* @email: Kinvy66@163.com
* @date: 2023/3/30 13:11
* @file: log.cpp
* @description: 
*/

#include "log.h"

namespace sylar {

const char* LogLevel::ToString(LogLevel::Level level) {
     switch (level) {
#define XX(name) \
     case LogLevel::name: \
         return #name;    \
         break;
     XX(DEBUG);
     XX(INFO);
     XX(WARN);
     XX(ERROR);
     XX(FATAL);
#undef XX
         default:
             return "UNKNOW";
     }
     return "UNKNOW";
}

class MessageFormatItem : public LoggerFormatter::FormatItem{
public:
    MessageFormatItem(const std::string& str="") {}
    void format(std::ostream  &os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {
        os << event->getContent();
    }
};

class LevelFormatItem : public LoggerFormatter::FormatItem{
public:
    LevelFormatItem(const std::string& str="") {}
    void format(std::ostream  &os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {
        os << LogLevel::ToString(level);
    }
};

class ElapseFormatItem : public LoggerFormatter::FormatItem{
public:
    ElapseFormatItem(const std::string& str="") {}
    void format(std::ostream  &os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {
        os << LogLevel::ToString(level);
    }
};

class NameFormatItem : public LoggerFormatter::FormatItem{
public:
    NameFormatItem(const std::string& str="") {}
    void format(std::ostream  &os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {
        os << logger->getName();
    }
};

class ThreadIdFormatItem : public LoggerFormatter::FormatItem{
public:
    ThreadIdFormatItem(const std::string& str="") {}
    void format(std::ostream  &os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {
        os << event->getThreadId();
    }
};

class FiberIdFormatItem : public LoggerFormatter::FormatItem{
public:
    FiberIdFormatItem(const std::string& str="") {}
    void format(std::ostream  &os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {
        os << event->getThreadId();
    }
};

class DateTimeFormatItem : public LoggerFormatter::FormatItem{
public:
    DateTimeFormatItem(const std::string& format = "%Y:%m:%d %H:%M:%S")
            : m_format(format) {

    }
    void format(std::ostream  &os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {
        os << event->getTime();
    }

private:
    std::string m_format;
};

class FileNameFormatItem : public LoggerFormatter::FormatItem{
public:
    FileNameFormatItem(const std::string& str="") {}
    void format(std::ostream  &os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {
        os << event->getFile();
    }
};

class LineFormatItem : public LoggerFormatter::FormatItem{
public:
    LineFormatItem(const std::string& str="") {}
    void format(std::ostream  &os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {
        os << event->gerLine();
    }
};

class NewLineFormatItem : public LoggerFormatter::FormatItem{
public:
    NewLineFormatItem(const std::string& str="") {}
    void format(std::ostream  &os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {
        os << std::endl;
    }
};

class StringFormatItem : public LoggerFormatter::FormatItem{
public:
    StringFormatItem(const std::string& str)
        : m_string(str) {

    }
    void format(std::ostream  &os, Logger::ptr logger, LogLevel::Level level, LogEvent::ptr event) override {
        os << m_string;
    }
private:
    std::string m_string;
};

LogEvent::LogEvent(const char *file, int32_t line, uint32_t elapse,
                   uint32_t thread_id, uint32_t fiber_id,uint64_t time)
                   : m_file(file)
                   ,m_line(line)
                   ,m_elapse(elapse)
                   ,m_threadId(thread_id)
                   ,m_fiberId(fiber_id)
                   ,m_time(time){

}


Logger::Logger(const std::string &name)
    : m_name(name)
    , m_level(LogLevel::DEBUG){
//    m_formatter.reset(new LoggerFormatter("%d{%Y-%m-%d %H:%M:%S}%T%t%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"));
    m_formatter.reset(new LoggerFormatter("%d [%p] %f %l %m %n"));
}

void Logger::addAppender(LogAppender::ptr appender) {
    if (!appender->getFormatter()) {
        appender->setFormatter(m_formatter);
    }
    m_appenders.push_back(appender);
}

void Logger::delAppender(LogAppender::ptr appender) {
    for (auto it = m_appenders.begin();
               it != m_appenders.end(); ++it) {
        if (*it == appender) {
            m_appenders.erase(it);
            break;
        }
    }
}

void Logger::log(LogLevel::Level level, LogEvent::ptr event) {
    if (level >= m_level) {
        auto self = shared_from_this();
        for (auto &i : m_appenders) {
            i->log(self, level, event);
        }
    }
}

void Logger::debug(LogEvent::ptr event) {
    log(LogLevel::DEBUG, event);
}

void Logger::info(LogEvent::ptr event) {
    log(LogLevel::INFO, event);
}

void Logger::warn(LogEvent::ptr event) {
    log(LogLevel::WARN, event);
}

void Logger::error(LogEvent::ptr event) {
    log(LogLevel::ERROR, event);
}

void Logger::fatal(LogEvent::ptr event) {
    log(LogLevel::FATAL, event);
}


FileLogAppender::FileLogAppender(const std::string &filename)
    : m_filename(filename){

}

void FileLogAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) {
    if(level >= m_level) {
        m_filestream << m_formatter->format(logger, level, event);
    }
}

bool FileLogAppender::reopen() {
    if (m_filestream) {
        m_filestream.close();
    }
    m_filestream.open(m_filename);
    return !m_filestream;
}

void StdoutAppender::log(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) {
    if (level >= m_level) {
        std::cout << m_formatter->format(logger, level, event);
    }
}

LoggerFormatter::LoggerFormatter(const std::string &pattern)
    :m_pattern(pattern){
    init();
}


std::string LoggerFormatter::format(std::shared_ptr<Logger> logger, LogLevel::Level level, LogEvent::ptr event) {
    std::stringstream  ss;
    for (auto &i : m_items) {
        i->format(ss, logger, level, event);
    }
    return ss.str();
}

// %xxx  %xxx(xxx)  %%
void LoggerFormatter::init() {
    // str, format, type  字符，格式，类型
    std::vector<std::tuple<std::string, std::string, int> > vec;
    std::string nstr;
    for(size_t i = 0; i < m_pattern.size(); ++i) {
        if(m_pattern[i] != '%') {
            nstr.append(1, m_pattern[i]);
            continue;
        }

        if((i + 1) < m_pattern.size()) {
            if(m_pattern[i + 1] == '%') {
                nstr.append(1, '%');
                continue;
            }
        }

        size_t n = i + 1;
        int fmt_status = 0;
        size_t fmt_begin = 0;

        std::string str;
        std::string fmt;
        while(n < m_pattern.size()) {
            if(!fmt_status && (!isalpha(m_pattern[n]) && m_pattern[n] != '{'
                               && m_pattern[n] != '}')) {
                str = m_pattern.substr(i + 1, n - i - 1);
                break;
            }
            if(fmt_status == 0) {
                if(m_pattern[n] == '{') {
                    str = m_pattern.substr(i + 1, n - i - 1);
                    //std::cout << "*" << str << std::endl;
                    fmt_status = 1; //解析格式
                    fmt_begin = n;
                    ++n;
                    continue;
                }
            } else if(fmt_status == 1) {
                if(m_pattern[n] == '}') {
                    fmt = m_pattern.substr(fmt_begin + 1, n - fmt_begin - 1);
                    //std::cout << "#" << fmt << std::endl;
                    fmt_status = 0;
                    ++n;
                    break;
                }
            }
            ++n;
            if(n == m_pattern.size()) {
                if(str.empty()) {
                    str = m_pattern.substr(i + 1);
                }
            }
        }

        if(fmt_status == 0) {
            if(!nstr.empty()) {
                vec.emplace_back(nstr, std::string(), 0);
                nstr.clear();
            }
            vec.emplace_back(str, fmt, 1);
            i = n - 1;
        } else if(fmt_status == 1) {
            std::cout << "pattern parse error: " << m_pattern << " - " << m_pattern.substr(i) << std::endl;
            vec.emplace_back("<<pattern_error>>", fmt, 0);
        }
    }

    if(!nstr.empty()) {
        vec.emplace_back(nstr, "", 0);
    }
    static std::map<std::string, std::function<FormatItem::ptr(const std::string& str)> > s_format_items = {
#define XX(str, C) \
        {#str, [](const std::string& fmt) { return FormatItem::ptr(new C(fmt));}}

            XX(m, MessageFormatItem),
            XX(p, LevelFormatItem),
            XX(r, ElapseFormatItem),
            XX(c, NameFormatItem),
            XX(t, ThreadIdFormatItem),
            XX(n, NewLineFormatItem),
            XX(d, DateTimeFormatItem),
            XX(f, FileNameFormatItem),
            XX(l, LineFormatItem),
//            XX(T, TabFormatItem),
            XX(F, FiberIdFormatItem),
#undef XX
    };

    for(auto& i : vec) {
        if(std::get<2>(i) == 0) {
            m_items.push_back(FormatItem::ptr(new StringFormatItem(std::get<0>(i))));
        } else {
            auto it = s_format_items.find(std::get<0>(i));
            if(it == s_format_items.end()) {
                m_items.push_back(FormatItem::ptr(new StringFormatItem("<<error_format %" + std::get<0>(i) + ">>")));
            } else {
                m_items.push_back(it->second(std::get<1>(i)));
            }
        }

        //std::cout << "(" << std::get<0>(i) << ") - (" << std::get<1>(i) << ") - (" << std::get<2>(i) << ")" << std::endl;
    }
    //std::cout << m_items.size() << std::endl;




/*    // str  format  type
    std::vector<std::tuple<std::string, std::string ,int>> vec;
//    size_t last_pos = 0;
    std::string nstr;
    for (size_t i = 0; i < m_pattern.size(); ++i) {
        if (m_pattern[i] != '%') {
            nstr.append(1, m_pattern[i]);
            continue;
        }
        if ((i+1) < m_pattern.size() ) {
            if(m_pattern[i+1] == '%') {
                nstr.append(1, '%');
                continue;
            }
        }

        size_t  n  = i + 1;
        int fmt_status = 0;
        size_t fmt_begin = 0;

        std::string  str;
        std::string  fmt;
        while( n < m_pattern.size()) {
            if (!isalpha(m_pattern[n]) && m_pattern[n] != '{'
                    && m_pattern[n] != '}') {   // 空格符就退出
                break;
            }
            if (fmt_status == 0) {
                if (m_pattern[n] == '{') {  // 左{
                    str = m_pattern.substr(i+1, n-i-1);
                    fmt_status = 1;     // 解析格式
                    ++n;
                    fmt_begin = n;
                    continue;
                }
            }
            if (fmt_status == 1) {
                if (m_pattern[n] == '}') {
                    fmt = m_pattern.substr(fmt_begin+1, n-fmt_begin-1);
                    fmt_status = 2;  // 结束解析
                    ++n;
                    break;
                }
            }
            ++n;
        }
        if (fmt_status == 0) {
            if (!nstr.empty()) {
                vec.push_back(std::make_tuple(nstr, "", 0));
                nstr.clear();
            }
            str = m_pattern.substr(i+1, n-i+1);
            vec.push_back(std::make_tuple(str, fmt, 1));
            i = n;
        } else if (fmt_status == 1) {  // 解析出错
            std::cout << "pattern parse error: "  << m_pattern << " - " << m_pattern.substr(i) << std::endl;
            vec.push_back(std::make_tuple("<<error>>", fmt, 0));
        } else if (fmt_status == 2) {  // 正常
            if (!nstr.empty()) {
                vec.push_back(std::make_tuple(nstr, "", 0));
                nstr.clear();
            }
            vec.push_back(std::make_tuple(str, fmt, 1));
            i = n;
        }
    }
    if (!nstr.empty()) {
        vec.push_back(std::make_tuple(nstr, "", 0));
    }
    static std::map<std::string, std::function<FormatItem::ptr(const std::string& str)>> s_format_items = {
#define XX(str, C) \
        {#str, [](const std::string& fmt) { return FormatItem::ptr (new MessageFormatItem(fmt));}}
        XX(m, MessageFormatItem),
        XX(p, LevelFormatItem),
        XX(r, ElapseFormatItem),
        XX(c, NameFormatItm),
        XX(t, ThreadIdFormatItm),
        XX(n, NewLineFormatItm),
        XX(d, DateTimeFormatItm),
        XX(f, FiberIdFormatItm),
        XX(l, LineFormatItm),
#undef XX
    };

    for( auto &i : vec) {
        if (std::get<2>(i) == 0) {
            m_items.push_back(FormatItem::ptr(new StringFormatItm(std::get<0>(i))));
        } else {
            auto it = s_format_items.find(std::get<0>(i));
            if(it == s_format_items.end()) {
                m_items.push_back(FormatItem::ptr(new StringFormatItm("<<error_format %" + std::get<0>(i) + ">>")));
            } else {
                m_items.push_back(it->second(std::get<1>(i)));
            }
        }
        std::cout << "{" << std::get<0>(i) << "} - {" <<std::get<1>(i) << "} - {" << std::get<2>(i) << "}" << std::endl;
    }
    std::cout << m_items.size() << std::endl;*/
    // %m -- 消息体
    // %p -- level
    // %r -- 启动后的时间
    // %c -- 日志名称
    // %t -- 线程id
    // %n -- 回车
    // %d -- 时间
    // %f -- 文件名
    // %l -- 行号
}






const char *m_file = nullptr;  // 文件名
int32_t m_line = 0;            // 行号
uint32_t m_elapse = 0;          // 程序启动到现在的毫秒数
int32_t m_threadId = 0;        //线程id
int32_t m_fiberId = 0;          // 协程id
int64_t m_time;                 // 时间戳
std::string m_content;          // 内容


}