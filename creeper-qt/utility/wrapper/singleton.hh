#pragma once
#include <memory>

namespace util {

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

}
