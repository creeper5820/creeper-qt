#pragma once

#include <tuple>

namespace creeper {

/// @brief
/// 函数签名提取工具，用于从成员函数指针中提取类型信息
///
/// @tparam T
///   成员函数指针类型
///
/// @note
/// 该 trait 用于支持 ForwardProp 从成员函数指针自动推导参数类型。
/// 提供了对 const、noexcept 等修饰符的完整支持。
///
/// 使用示例：
/// @code
///     using traits = function_traits<decltype(&QWidget::setText)>;
///     using arg_type = traits::template arg<0>; // QString
///     static_assert(traits::arity == 1);
/// @endcode
///
template <typename T>
struct function_traits;

// 成员函数指针：非 const
template <typename R, typename C, typename... Args>
struct function_traits<R (C::*)(Args...)> {
    using return_type = R;
    using class_type  = C;

    static constexpr std::size_t arity = sizeof...(Args);

    template <std::size_t N>
    using arg = std::tuple_element_t<N, std::tuple<Args...>>;

    using args_tuple = std::tuple<Args...>;
};

// 成员函数指针：const
template <typename R, typename C, typename... Args>
struct function_traits<R (C::*)(Args...) const> : function_traits<R (C::*)(Args...)> { };

// 成员函数指针：noexcept
template <typename R, typename C, typename... Args>
struct function_traits<R (C::*)(Args...) noexcept> : function_traits<R (C::*)(Args...)> { };

// 成员函数指针：const noexcept
template <typename R, typename C, typename... Args>
struct function_traits<R (C::*)(Args...) const noexcept> : function_traits<R (C::*)(Args...)> { };

}
