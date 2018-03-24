//
//  atomic_types.h
//  mmth1803
//
//  Created by WOOHYUN RIM on 2018-03-24.
//  Copyright © 2018 Protelum Liberium. All rights reserved.
//

#ifndef atomic_types_h
#define atomic_types_h

#include <mutex>
#include <boost/multiprecision/cpp_int.hpp>


template <class Class>
class atomic{
    Class _data;
    mutable std::mutex _mutex;
public:
    void lock(){
        _mutex.lock();
    }
    void unlock(){
        _mutex.unlock();
    }
    void raw_assign(const Class& op){
        _data = op;
    }
    
    constexpr atomic() = default;
    constexpr atomic(const atomic& op): _data(op._data){}
    constexpr atomic(const Class& op):_data(op){}
    atomic(Class&& op):_data(op){}
    operator Class() const{
        _mutex.lock();
        Class result = _data;
        _mutex.unlock();
        return result;
    }
        
    atomic& operator = (const Class& op){
        std::lock_guard<std::mutex> lock(_mutex);
        _data = op;
    }
    atomic& operator = (Class&& op){
        std::lock_guard<std::mutex> lock(_mutex);
        _data = op;
    }
    atomic& operator ++ (){
        std::lock_guard<std::mutex> lock(_mutex);
        ++_data;
    }
    Class operator ++ (int){
        std::lock_guard<std::mutex> lock(_mutex);
        return _data++;
    }
    atomic& operator -- (){
        std::lock_guard<std::mutex> lock(_mutex);
        --_data;
    }
    Class operator -- (int){
        std::lock_guard<std::mutex> lock(_mutex);
        return _data--;
    }
    atomic& operator += (const Class& op){
        std::lock_guard<std::mutex> lock(_mutex);
        _data += op;
        return *this;
    }
    atomic& operator -= (const Class& op){
        std::lock_guard<std::mutex> lock(_mutex);
        _data -= op;
        return *this;
    }
    atomic& operator *= (const Class& op){
        std::lock_guard<std::mutex> lock(_mutex);
        _data *= op;
        return *this;
    }
    atomic& operator /= (const Class& op){
        std::lock_guard<std::mutex> lock(_mutex);
        _data /= op;
        return *this;
    }
};


#endif /* atomic_types_h */
