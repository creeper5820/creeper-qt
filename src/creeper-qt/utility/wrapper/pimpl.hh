#pragma once

#include <memory>

#define CREEPER_PIMPL_DEFINITION(Class)                                                            \
public:                                                                                            \
    Class();                                                                                       \
    ~Class();                                                                                      \
    Class(const Class&)            = delete;                                                       \
    Class& operator=(const Class&) = delete;                                                       \
                                                                                                   \
private:                                                                                           \
    struct Impl;                                                                                   \
    std::unique_ptr<Impl> pimpl;

namespace creeper::internal {
inline void use_std_unique_ptr() { auto unique_ptr = std::unique_ptr<int> {}; }
}
