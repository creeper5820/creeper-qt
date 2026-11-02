#pragma once

#include <memory> // IWYU pragma: keep

#define CREEPER_PIMPL_DEFINITION(Class)                                                            \
public:                                                                                            \
    Class();                                                                                       \
    ~Class();                                                                                      \
    Class(const Class&)            = delete;                                                       \
    Class(Class&&)                 = delete;                                                       \
    Class& operator=(const Class&) = delete;                                                       \
    Class& operator=(Class&&)      = delete;                                                       \
                                                                                                   \
private:                                                                                           \
    struct Impl;                                                                                   \
    std::unique_ptr<Impl> pimpl;
