/**
* @file noncopyable.h
* @brief 不可拷贝对象封装
* @author Kinvy
* @email Kinvy66@163.com
* @date 2023-5-11
*/
#ifndef __SYLAR_NONCOPYABLE_H
#define __SYLAR_NONCOPYABLE_H

namespace sylar {

/**
 * @brief 对象无法拷贝,赋值
 */
class Noncopyable {
public:
    /**
     * @brief 默认构造函数
     */
    Noncopyable() = default;

    /**
     * @brief 默认析构函数
     */
    ~Noncopyable() = default;

    /**
     * @brief 拷贝构造函数(禁用)
     */
    Noncopyable(const Noncopyable&) = delete;

    /**
     * @brief 赋值函数(禁用)
     */
    Noncopyable& operator=(const Noncopyable&) = delete;
};

}

#endif //__SYLAR_NONCOPYABLE_H
