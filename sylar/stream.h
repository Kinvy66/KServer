/**
* @file: stream.h
* @brief: stream 封装
* @author: Kinvy.Qiu
* @email: Kinvy66@163.com
* @date: 2023/5/31 
*/
#ifndef __SYLAR_STREAM_H
#define __SYLAR_STREAM_H

#include <memory>
#include "bytearray.h"

namespace sylar {

class Stream {
public:
    using ptr = std::shared_ptr<Stream>;
    virtual ~Stream() {}

    virtual int read(void* buffer, size_t length) = 0;
    virtual int read(ByteArray::ptr ba, size_t length) = 0;
    virtual int readFixSize(void* buffer, size_t length);
    virtual int readFixSize(ByteArray::ptr ba, size_t length);

    virtual int write(const void* buffer, size_t length) = 0;
    virtual int write(ByteArray::ptr ba, size_t length) = 0;
    virtual int writeFixSize(const void* buffer, size_t length);
    virtual int writeFixSize(ByteArray::ptr ba, size_t length);
    virtual void close() = 0;

};

}

#endif //__SYLAR_STREAM_H
