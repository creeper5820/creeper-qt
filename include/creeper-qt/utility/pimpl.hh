#pragma once

#define CREEPER_PIMPL_DEFINTION(Class)                                                             \
public:                                                                                            \
    Class();                                                                                       \
    ~Class();                                                                                      \
    Class(const Class&) = delete;                                                                  \
    Class& operator=(const Class&) = delete;                                                       \
                                                                                                   \
private:                                                                                           \
    struct Impl;                                                                                   \
    Impl* pimpl_;

#define CREEPER_WIDGET_PIMPL_DEFINTION(Class)                                                      \
public:                                                                                            \
    Class(QWidget* parent = nullptr);                                                              \
    ~Class();                                                                                      \
    Class(const Class&) = delete;                                                                  \
    Class& operator=(const Class&) = delete;                                                       \
                                                                                                   \
private:                                                                                           \
    struct Impl;                                                                                   \
    Impl* pimpl_;
