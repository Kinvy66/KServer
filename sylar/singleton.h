/**
* @author: Kinvy
* @email: Kinvy66@163.com
* @date: 2023/3/31 20:01
* @file: singleton.h
* @description: 
*/
#ifndef __SYLAR_SINGLETON_H
#define __SYLAR_SINGLETON_H

#include <memory>

namespace sylar {
template<typename T, typename X = void ,int N = 0>
class Singleton {
public:
    static T* GetInstance() {
        static T v;
        return &v;
    }
};

template<typename T, typename X = void , int N = 0>
class SingletonPtr {
public:
    static std::shared_ptr<T> GetInstance(){
       static std::shared_ptr<T> v(new T);
        return v;
    }
};

}


#endif //__SYLAR_SINGLETON_H
