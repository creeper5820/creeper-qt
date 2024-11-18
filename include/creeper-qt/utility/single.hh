#pragma once
#include <memory>

namespace creeper {

template <typename T> class Singleton {
public:
    static T& instance();

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton) = delete;

protected:
    struct token { };
    Singleton() = default;
};

template <typename T> inline T& Singleton<T>::instance() {
    static const std::unique_ptr<T> instance { new T { token {} } };
    return *instance;
}

#define SingletonConstructor(T) T(creeper::Singleton<T>::token)
#define SingletonDestructor(T) ~T()
#define RemoveCopyConstructor(T)                                               \
    T(const T&) = delete;                                                      \
    T& operator=(const T) = delete;

}