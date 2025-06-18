#pragma once

/// @note 此处不使用 auto... 来作为构造参数，有体谅语法提示的考量，
///         为了更有效的编写期检查，还是谨慎地提供构造类型吧
#define CREEPER_DEFINE_CONSTROCTOR(CLASS, NAMESPACE)                                               \
public:                                                                                            \
    explicit CLASS(NAMESPACE::property_concept auto... properties) {                               \
        (apply(std::forward<decltype(properties)>(properties)), ...);                              \
    }                                                                                              \
    template <NAMESPACE::property_concept... Args>                                                 \
    explicit CLASS(                                                                                \
        const std::tuple<Args...>& tuple, NAMESPACE::property_concept auto... properties) {        \
        apply(tuple), (apply(std::forward<decltype(properties)>(properties)), ...);                \
    }                                                                                              \
    template <NAMESPACE::property_concept... Args> void apply(const std::tuple<Args...>& _) {      \
        std::apply(                                                                                \
            [this](auto&&... args) { (apply(std::forward<decltype(args)>(args)), ...); }, _);      \
    }                                                                                              \
    void apply(const NAMESPACE::property_concept auto& property) { property.apply(*this); }
