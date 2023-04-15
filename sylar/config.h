/**
* @author: Kinvy
* @email: Kinvy66@163.com
* @date: 2023/4/2 14:28
* @file: config.h
* @description: 
*/
#ifndef __SYLAR_CONFIG_H
#define __SYLAR_CONFIG_H

#include <memory>
#include <string>
#include <boost/lexical_cast.hpp>
#include <boost/lexical_cast.hpp>
#include "log.h"
#include "yaml-cpp/yaml.h"
#include <vector>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <functional>



namespace sylar {

/**
 * @brief 配置变量的基类
 */
class ConfigVarBase {
public:
    typedef std::shared_ptr<ConfigVarBase> ptr;
    /**
     * @brief 构造函数
     * @param name 配置参数名称[0-9a-z_.]
     * @param description 配置参数描述
     */
    ConfigVarBase(const std::string& name, const std::string description = "")
        : m_name(name)
        , m_description(description) {
        // 忽略大小写，统一转换为小写
        std::transform(m_name.begin(), m_name.end(), m_name.begin(), ::tolower);
    }

    /**
     * @brief 虚构函数
     */
    virtual ~ConfigVarBase() = default;

    /**
     * @brief 返回配置参数名称
     * @return 配置参数名称
     */
    const std::string& getName() const { return m_name; }

    /**
     * @brief 返回配置参数的描述
     */
    const std::string& getDescription() const { return m_description; }

    /**
     * @brief 转换成字符串
     */
    virtual std::string  toString() = 0;

    /**
     * @brief 从字符串初始化值
     * @param val 初始化的字符串
     */
    virtual bool fromString(const std::string& val) = 0;

    /**
     * @brief 返回配置参数值的类型名称
     */
    virtual std::string getTypeName() const = 0;


protected:
    /// 配置参数的名称
    std::string m_name;
    /// 配置参数的描述
    std::string m_description;
};

// F from type, T to type
/**
 * @brief 类型转换模板类
 * @tparam F 源类型
 * @tparam T 目标类型
 */
template<typename F, typename T>
class LexicalCast {
public:
    /**
     * @brief 类型转换
     * @param v 源类型值
     * @return 返回v转换后的目标类型
     * @exception 当类型不可转换时抛出异常
     */
    T operator()(const F& v) {
        return boost::lexical_cast<T>(v);
    }
};

/**
 * @brief 类型转换模板类偏特化(YAML String 转换成 std::vector<T>)
 */
template<typename T>
class LexicalCast<std::string, std::vector<T> > {
public:
    std::vector<T> operator()(const std::string& v) {
        YAML::Node node = YAML::Load(v);
        typename std::vector<T> vec;
        std::stringstream  ss;
        for (size_t i = 0; i < node.size(); ++i) {
            ss.str("");
            ss << node[i];
            vec.push_back(LexicalCast<std::string, T>()(ss.str()));
        }
        return vec;
    }
};

/**
 * @brief 类型转换模板类片特化(std::vector<T> 转换成 YAML String)
 */
template<typename T>
class LexicalCast<std::vector<T>, std::string> {
public:
    std::string operator()(const std::vector<T>& v) {
        YAML::Node node;
        for (auto& i :v) {
            node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();

    }
};

/**
 * @brief 类型转换模板类片特化(YAML String 转换成 std::list<T>)
 */
template<typename T>
class LexicalCast<std::string, std::list<T> > {
public:
    std::list<T> operator()(const std::string& v) {
        YAML::Node node = YAML::Load(v);
        typename std::list<T> vec;
        std::stringstream  ss;
        for (size_t i = 0; i < node.size(); ++i) {
            ss.str("");
            ss << node[i];
            vec.push_back(LexicalCast<std::string, T>()(ss.str()));
        }
        return vec;
    }
};

/**
 * @brief 类型转换模板类片特化(std::list<T> 转换成 YAML String)
 * @tparam T
 */
template<typename T>
class LexicalCast<std::list<T>, std::string> {
public:
    std::string operator()(const std::list<T>& v) {
        YAML::Node node;
        for (auto& i :v) {
            node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();

    }
};

/**
 * @brief  类型转换模板类片特化(YAML String 转换成 std::set<T>)
 */
template<typename T>
class LexicalCast<std::string, std::set<T> > {
public:
    std::set<T> operator()(const std::string& v) {
        YAML::Node node = YAML::Load(v);
        typename std::set<T> vec;
        std::stringstream  ss;
        for (size_t i = 0; i < node.size(); ++i) {
            ss.str("");
            ss << node[i];
            vec.insert(LexicalCast<std::string, T>()(ss.str()));
        }
        return vec;
    }
};

/**
 * @brief 类型转换模板类片特化(std::set<T>  转换成 YAML String)
 * @tparam
 */
template<typename T>
class LexicalCast<std::set<T>, std::string> {
public:
    std::string operator()(const std::set<T>& v) {
        YAML::Node node;
        for (auto& i :v) {
            node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();

    }
};


/**
 * @brief 类型转换模板类片特化(YAML String 转换成 std::unordered_set<T>)
 */
template<typename T>
class LexicalCast<std::string, std::unordered_set<T> > {
public:
    std::unordered_set<T> operator()(const std::string& v) {
        YAML::Node node = YAML::Load(v);
        typename std::unordered_set<T> vec;
        std::stringstream  ss;
        for (size_t i = 0; i < node.size(); ++i) {
            ss.str("");
            ss << node[i];
            vec.insert(LexicalCast<std::string, T>()(ss.str()));
        }
        return vec;
    }
};

/**
 * @brief 类型转换模板类片特化(std::unordered_set<T> 转换成 YAML String)
 */
template<typename T>
class LexicalCast<std::unordered_set<T>, std::string> {
public:
    std::string operator()(const std::unordered_set<T>& v) {
        YAML::Node node;
        for (auto& i :v) {
            node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();

    }
};

/**
 * @brief 类型转换模板类片特化(YAML String 转换成 std::map<std::string, T>)
 * @tparam T
 */
template<typename T>
class LexicalCast<std::string, std::map<std::string, T> > {
public:
    std::map<std::string, T>  operator()(const std::string& v) {
        YAML::Node node = YAML::Load(v);
        typename std::map<std::string, T>  vec;
        std::stringstream  ss;
        for (auto it = node.begin();
                    it != node.end(); ++it) {
            ss.str("");
            ss << it->second;
            vec.insert(std::make_pair(it->first.Scalar(),
                                      LexicalCast<std::string, T>()(ss.str())));
        }
        return vec;
    }
};

/**
 * @brief 类型转换模板类片特化(std::map<std::string, T> 转换成 YAML String)
 */
template<typename T>
class LexicalCast< std::map<std::string, T>, std::string> {
public:
    std::string operator()(const  std::map<std::string, T>& v) {
        YAML::Node node;
        for (auto& i :v) {
            node[i.first] = YAML::Load(LexicalCast<T, std::string>()(i.second));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();

    }
};

/**
 * @brief 类型转换模板类片特化(YAML String 转换成 std::unordered_map<std::string, T>)
 */
template<typename T>
class LexicalCast<std::string, std::unordered_map<std::string, T> > {
public:
    std::unordered_map<std::string, T>  operator()(const std::string& v) {
        YAML::Node node = YAML::Load(v);
        typename std::unordered_map<std::string, T>  vec;
        std::stringstream  ss;
        for (auto it = node.begin();
             it != node.end(); ++it) {
            ss.str("");
            ss << it->second;
            vec.insert(std::make_pair(it->first.Scalar(),
                                      LexicalCast<std::string, T>()(ss.str())));
        }
        return vec;
    }
};

/**
 * @brief 类型转换模板类片特化(std::unordered_map<std::string, T> 转换成 YAML String)
 */
template<typename T>
class LexicalCast< std::unordered_map<std::string, T>, std::string> {
public:
    std::string operator()(const  std::unordered_map<std::string, T>& v) {
        YAML::Node node;
        for (auto& i :v) {
            node[i.first] = YAML::Load(LexicalCast<T, std::string>()(i.second));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();

    }
};

// FromStr T operator()(const std::string&)
// ToStr std::string operator()(const T&)
// 目标特例化
/**
 * @brief 配置参数模板子类,保存对应类型的参数值
 * @tparam T 参数的具体类型
 * @tparam FromStr 从std::string转换成T类型的仿函数
 * @tparam ToStr  从T转换成std::string的仿函数
 * @details
 *          继承自ConfigVarBase的模板类，
 *          实现类型 T 和 字符串(std:string)之间转换
 *          std::string 为YAML格式的字符串
 *          toString 和 fromString 的实现是调用函数对象 LexicalCast< T, F>
 *
 */
template<typename T, typename FromStr = LexicalCast<std::string, T>,
        typename ToStr = LexicalCast<T, std::string> >
class ConfigVar : public ConfigVarBase {
public:
    typedef std::shared_ptr<ConfigVar> ptr;
    typedef std::function<void (const T& old_value, const T& new_value)> on_change_cb;

    /**
     * @brief 通过参数名,参数值,描述构造ConfigVar
     * @param name 参数名称有效字符为[0-9a-z_.]
     * @param default_value 参数的默认值
     * @param description 参数的描述
     */
    ConfigVar(const std::string& name,
              const T& default_value,
              const std::string& description = "")
              : ConfigVarBase(name, description)
              , m_val{default_value} {

    }

    /**
     * @brief 将参数值转换成YAML String
     * @exception 当转换失败抛出异常
     */
    std::string toString() override {
        try {
            // return boost::lexical_cast<std::string>(m_val);
            return ToStr()(m_val);
        } catch (std::exception& e) {
            SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "ConfigVar::toString exception"
                << e.what() << " convert: " << typeid(m_val).name() << " to string";
        }
        return "";
    }

    /**
     * @brief 从YAML String 转成参数的值，并设置到对应的配置项
     * @param val YAML string 字符串
     * @exception 当转换失败抛出异常
     */
    bool fromString(const std::string& val) override {
        try {
            // m_val = boost::lexical_cast<T>(val);
            setValue(FromStr()(val));
        } catch (std::exception& e) {
            SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "ConfigVar::toString exception"
            << e.what() << " convert: " << typeid(m_val).name() << " to string";
        }
        return false;
    }

    /**
     * @brief 获取当前参数的值
     */
    const T getValue() const { return m_val; }

    /**
     * @brief 设置当前参数的值
     * @param v 设置值
     * @details 如果参数的值有发生变化,则通知对应的注册回调函数
     */
    void setValue(const T& v) {
        if (v == m_val) {
            return;
        }
        for (auto& i : m_cbs) {
            i.second(m_val, v);
        }
        m_val = v;
    }

    /**
     * @brief 返回参数值的类型名称(typeinfo)
     */
    std::string getTypeName() const override { return typeid(T).name(); }

    // TODO: 该方法需要改动
    /**
     * @brief 添加变化回调函数
     * @param key
     * @param cb
     */
    void addListener(uint64_t key, on_change_cb cb) {
        m_cbs[key] = cb;
    }

    /**
     * @brief 删除回调函数
     * @param key 回调函数的唯一id
     */
    void delListener(uint64_t key) {
        m_cbs.erase(key);
    }

    /**
     * @brief 获取回调函数
     * @param key 回调函数的唯一id
     * @return 如果存在返回对应的回调函数,否则返回nullptr
     */
    on_change_cb getListener(uint64_t key) {
        auto it = m_cbs.find(key);
        return it == m_cbs.end() ? nullptr : it->second;
    }

    /**
     * @brief 清理所有的回调函数
     */
    void clearListener() {
        m_cbs.clear();
    }

private:
    T m_val;
    // 变更回调函数数组，uint64_t key: 要求唯一，一般用hash
    std::map<uint64_t , on_change_cb> m_cbs;
};

/**
 * @brief ConfigVar的管理类
 * @details 供便捷的方法创建/访问ConfigVar
 */
class Config {
public:
    typedef std::unordered_map<std::string, ConfigVarBase::ptr> ConfigVarMap;

    /**
     * @brief  获取/创建对应参数名的配置参数
     * @tparam T
     * @param name 配置参数名称
     * @param default_value 参数默认值
     * @param description 参数描述
     * @details 获取参数名为name的配置参数,如果存在直接返回
     *          如果不存在,创建参数配置并用default_value赋值
     * @return 返回对应的配置参数,如果参数名存在但是类型不匹配则返回nullptr
     * @exception 如果参数名包含非法字符[^0-9a-z_.] 抛出异常 std::invalid_argument
     */
    template<typename T>
    static typename ConfigVar<T>::ptr Lookup(const std::string& name,
                                             const T& default_value,
                                             const std::string& description = "") {
        auto it = GetDatas().find(name);
        if (it != GetDatas().end()) {
            auto tmp = std::dynamic_pointer_cast<ConfigVar<T>>(it->second);
            if (tmp) {    // 同名配置存在，直接返回
                SYLAR_LOG_INFO(SYLAR_LOG_ROOT()) << "Lookup name=" << name << " exists";
                return tmp;
            } else {     // 同名配置存在，但类型不匹配，输出错误日志
                SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "Lookup name=" << name << " exists but type not "
                        << typeid(T).name() << " real_type=" << it->second->getTypeName()
                        << " " << it->second->toString();
                return nullptr;
            }
        }

        if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyz._0123456789")
                != std::string::npos) {
            SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "Lookup name invalid " << name;
            throw std::invalid_argument(name);
        }

        // 不存在的配置，添加
        typename ConfigVar<T>::ptr  v(new ConfigVar<T>(name, default_value, description));
        GetDatas()[name] = v;
        return v;
    }

    /**
     * @brief 查找配置参数
     * @tparam T
     * @param name 配置参数名称
     * @return 返回配置参数名为name的配置参数
     */
    template<typename T>
    static typename ConfigVar<T>::ptr Lookup(const std::string& name) {
        auto it = GetDatas().find(name);
        if (it == GetDatas().end()) {
            return nullptr;
        }
        return std::dynamic_pointer_cast<ConfigVar<T>>(it->second);
    }

    /**
     * @brief 使用YAML::Node初始化配置模块
     */
    static void LoadFromYaml(const YAML::Node &root);

    /**
     * @brief 查找配置参数,返回配置参数的基类
     * @param name 配置参数名称
     */
    static ConfigVarBase::ptr LookupBase(const std::string& name);

private:
    static ConfigVarMap& GetDatas() {
        static ConfigVarMap s_datas;
        return s_datas;
    }
};

}

#endif //__SYLAR_CONFIG_H
