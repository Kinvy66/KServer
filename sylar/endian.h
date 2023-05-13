/**
* @file: endian.h
* @brief: 字节序转换
* @author: Kinvy.Qiu
* @email: Kinvy66@163.com
* @date: 2023/5/13 
*/
#ifndef __SYLAR_ENDIAN_H
#define __SYLAR_ENDIAN_H

#define SYLAR_LITTLE_ENDIAN     1
#define SYLAR_BIG_ENDIAN        2

#include <byteswap.h>
#include <stdint.h>
#include <type_traits>

namespace sylar {

template<typename T>
typename std::enable_if<sizeof(T) == sizeof(uint64_t), T>::type
byteswap(T value) {
    return (T) bswap_64((uint64_t)value);
}

template<typename T>
typename std::enable_if<sizeof(T) == sizeof(uint32_t), T>::type
byteswap(T value) {
    return (T) bswap_32((uint32_t)value);
}

template<typename T>
typename std::enable_if<sizeof(T) == sizeof(uint16_t), T>::type
byteswap(T value) {
    return (T) bswap_16((uint16_t)value);
}

#if BYTE_ORDER == BIG_ENDIAN
#define SYLAR_BYTE_ORDER SYLAR_BIG_ENDIAN
#else
#define SYLAR_BYTE_ORDER SYLAR_LITTLE_ENDIAN
#endif

#if SYLAR_BYTE_ORDER == SYLAR_BIG_ENDIAN
template<typename T>
T byteswapOnLittleEndian(T t) {
    return t;
}

template<typename T>
T byteswapOnBigEndian(T t) {
    return byteswap(t);
}

#else
template<typename T>
T byteswapOnLittleEndian(T t) {
    return byteswap(t);
}

template<typename T>
T byteswapOnBigEndian(T t) {
    return t;
}

#endif

}

#endif //__SYLAR_ENDIAN_H
