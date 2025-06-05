#pragma once

#include <qbitmap.h>
#include <qicon.h>
#include <qwidget.h>

/// @note 此处不使用 auto... 来作为构造参数，有体谅语法提示的考量，
///         为了更有效的编写期检查，还是谨慎地提供构造类型吧
/// @param INIT 嗯... 有时候你不得不占用构造函数，这就和 pimpl 模式不兼容了，只好添加一个 init
///         函数作为补丁了
#define CREEPER_DEFINE_CONSTROCTOR(CLASS, NAMESPACE, INIT)                                         \
public:                                                                                            \
    explicit CLASS(NAMESPACE::property_concept auto... properties) {                               \
        (apply(std::forward<decltype(properties)>(properties)), ...);                              \
        INIT();                                                                                    \
    }                                                                                              \
    template <NAMESPACE::property_concept... Args>                                                 \
    explicit CLASS(                                                                                \
        const std::tuple<Args...>& tuple, NAMESPACE::property_concept auto... properties) {        \
        apply(tuple), (apply(std::forward<decltype(properties)>(properties)), ...);                \
        INIT();                                                                                    \
    }                                                                                              \
    template <NAMESPACE::property_concept... Args> void apply(const std::tuple<Args...>& _) {      \
        std::apply(                                                                                \
            [this](auto&&... args) { (apply(std::forward<decltype(args)>(args)), ...); }, _);      \
    }                                                                                              \
    void apply(const NAMESPACE::property_concept auto& _) {                                        \
        static_cast<const NAMESPACE::Property&>(_).apply(*this);                                   \
    }

namespace creeper {

template <typename Instance> struct InternalProperty {
    virtual void apply(Instance& _) const = 0;
};

}
