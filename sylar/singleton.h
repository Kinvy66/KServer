/**
* @file singleton.h
* @brief 单例模式封装
* @author Kinvy
* @email Kinvy66@163.com
* @date 2023-3-31
*/
#ifndef __SYLAR_SINGLETON_H
#define __SYLAR_SINGLETON_H

#include <memory>

namespace sylar {

 /**
  * @brief 单例模式封装类
  * @tparam T 类型
  * @tparam X 为了创造多个实例对应的Tag
  * @tparam N 同一个Tag创造多个实例索引
  */
template<typename T, typename X = void ,int N = 0>
class Singleton {
public:
    /**
     * @brief 返回单例裸指针
     */
    static T* GetInstance() {
        static T v;
        return &v;
    }
};

/**
 * @brief 单例模式智能指针封装类
 * @tparam T 类型
 * @tparam X 为了创造多个实例对应的Tag
 * @tparam N 同一个Tag创造多个实例索引
 */
template<typename T, typename X = void , int N = 0>
class SingletonPtr {
public:
    /**
     * @brief 返回单例智能指针
     */
    static std::shared_ptr<T> GetInstance(){
       static std::shared_ptr<T> v(new T);
        return v;
    }
};

}

#endif //__SYLAR_SINGLETON_H
