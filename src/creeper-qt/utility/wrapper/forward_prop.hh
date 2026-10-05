#pragma once

#include <utility>

#include "function_traits.hh"

namespace creeper {

/// @brief
/// 从成员函数指针自动生成声明式属性
///
/// @tparam MemberFuncPtr
///   成员函数指针，例如 &QWidget::setText
///
/// @note
/// ForwardProp 通过成员函数指针自动推导参数类型，生成对应的声明式属性。
/// 相比手写 SetterProp，ForwardProp 消除了大量样板代码，同时保持类型安全。
///
/// 使用示例：
/// @code
///     namespace api::scope::widget {
///         using Text = ForwardProp<&QWidget::setText>;
///         using FixedWidth = ForwardProp<&QWidget::setFixedWidth>;
///     }
///
///     MyWidget {
///         api::scope::widget::Text { "Hello" },
///         api::scope::widget::FixedWidth { 200 },
///     }
/// @endcode
///
/// @note
/// 该实现支持三种场景：
/// - 单参数成员函数：存储参数值，apply 时调用
/// - 无参数成员函数：无需存储，apply 时直接调用
/// - 多参数成员函数：存储 tuple，apply 时展开调用
///
template <auto MemberFuncPtr>
struct ForwardProp;

/// @brief
/// 单参数版本：存储参数值
///
template <auto MemberFuncPtr>
    requires(function_traits<decltype(MemberFuncPtr)>::arity == 1)
struct ForwardProp<MemberFuncPtr> {
    using traits     = function_traits<decltype(MemberFuncPtr)>;
    using value_type = std::remove_cvref_t<typename traits::template arg<0>>;

    value_type value;

    // 完美转发构造
    constexpr ForwardProp(auto&& v) noexcept
        requires std::constructible_from<value_type, decltype(v)>
        : value(std::forward<decltype(v)>(v)) { }

    // 默认构造
    constexpr ForwardProp() noexcept
        requires std::default_initializable<value_type>
    = default;

    // 赋值运算符
    template <typename O>
    auto operator=(O&& other) noexcept -> ForwardProp&
        requires std::assignable_from<value_type&, O>
    {
        value = std::forward<O>(other);
        return *this;
    }

    // 类型转换
    constexpr explicit operator value_type(this auto&& self) noexcept {
        return std::forward<decltype(self)>(self).value;
    }

    // dsl_invoke 通过 ADL 查找
    friend auto dsl_invoke(auto& widget, const ForwardProp& prop) -> void {
        (widget.*MemberFuncPtr)(prop.value);
    }
};

/// @brief
/// 无参数版本：无需存储，直接调用
///
template <auto MemberFuncPtr>
    requires(function_traits<decltype(MemberFuncPtr)>::arity == 0)
struct ForwardProp<MemberFuncPtr> {
    // dsl_invoke 通过 ADL 查找
    friend auto dsl_invoke(auto& widget, const ForwardProp&) -> void { (widget.*MemberFuncPtr)(); }
};

/// @brief
/// 多参数版本：存储 tuple，apply 时展开
///
template <auto MemberFuncPtr>
    requires(function_traits<decltype(MemberFuncPtr)>::arity > 1)
struct ForwardProp<MemberFuncPtr> {
    using traits     = function_traits<decltype(MemberFuncPtr)>;
    using args_tuple = typename traits::args_tuple;

    args_tuple values;

    // 构造函数：接受所有参数
    template <typename... Args>
    constexpr ForwardProp(Args&&... args) noexcept
        requires std::constructible_from<args_tuple, Args...>
        : values(std::forward<Args>(args)...) { }

    // 默认构造
    constexpr ForwardProp() noexcept
        requires std::default_initializable<args_tuple>
    = default;

    // dsl_invoke 通过 ADL 查找
    friend auto dsl_invoke(auto& widget, const ForwardProp& prop) -> void {
        std::apply(
            [&widget](
                auto&&... args) { (widget.*MemberFuncPtr)(std::forward<decltype(args)>(args)...); },
            prop.values);
    }
};

}
