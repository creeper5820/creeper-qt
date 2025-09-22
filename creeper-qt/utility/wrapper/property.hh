#pragma once

#include <tuple>
#include <utility>

// 少量样板代码的生成宏是可以接受的，
// 等 concept 可以作为参数的那一天，这个宏就可以废弃了
#define CREEPER_DEFINE_CHECKER(TRAIT)                                                              \
    struct checker final {                                                                         \
        template <class T>                                                                         \
        static constexpr bool result = TRAIT<T>;                                                   \
    };

namespace creeper {

/// CHECKER
template <class T>
concept checker_trait = requires {
    { T::template result<void> };
};

template <checker_trait... Ts>
struct CheckerOr {
    template <class T>
    static constexpr bool result = (Ts::template result<T> || ...);
};
template <checker_trait... Ts>
struct CheckerAnd {
    template <class T>
    static constexpr bool result = (Ts::template result<T> && ...);
};

/// PROP
template <class Token, typename T, auto interface>
struct SetterProp : Token {
    T value;

    constexpr explicit SetterProp(T value) noexcept
        : value { std::move(value) } { }

    constexpr explicit operator T(this auto&& self) noexcept { return self.value; }

    template <typename O>
    auto operator=(O&& other) noexcept -> SetterProp&
        requires std::assignable_from<T&, O>
    {
        value = std::forward<O>(other);
        return *this;
    }

    auto apply(auto& self) const noexcept -> void
        requires requires { interface(self, std::declval<T>()); }
    {
        interface(self, value);
    }
};
template <class Token, typename T, auto interface>
struct DerivedProp : T, Token {
    using T::T;
    explicit DerivedProp(const T& value)
        : T(value) { }
    auto apply(auto& self) const -> void { interface(self, *this); }
};
template <class Token, auto interface>
struct ActionProp : Token {
    auto apply(auto& self) const noexcept -> void
        requires requires { interface(self); }
    {
        interface(self);
    }
};

/// @brief
/// 声明式包装，非侵入式实现 Setter 的声明式化
///
/// 该包装器支持将 std::tuple 与单个 prop 混合传入，不受顺序限制。内部通过
/// helper 检查 tuple 中的所有元素，递归调用 apply 将属性应用到 底层 Widget。
/// 利用 CheckerT 延迟模板实例化，在 concept 层面约束属性类型， 从而避免因
/// 递归参数展开而产生的海量且难以定位的模板错误。
///
/// @tparam W
///   需要被包装的组件类型。
///
/// @tparam checker
///   用于延迟模板实例化的“检查器”类型模板。典型形式如下：
/// struct checker final {
///     template <class T> struct result {
///         static constexpr auto v = concept<T>;
///     };
/// };
///
/// @note
/// 在不符合 checker 要求的类型被传入时，会在 concept 约束阶段直接报错，
/// 提供简洁且精准的编译期错误提示，避免编译器自动展开大量构造函数
/// 导致的冗长错误栈，但编译期报错信息依旧不友好。
///
template <class W, checker_trait checker>
struct Declarative : public W {
private:
    // For helpe check single props
    // 这里没法子塞一个 static_assert，无论如何这里都会被尝试错误地实例化
    template <typename T, class checker_>
    static constexpr auto impl_props_trait = checker_::template result<std::remove_cvref_t<T>>;

    // For help check tuple of props
    // 使用 SFINAE 真是抱歉，没找到方便处理 tuple 的方法真是不好意思呢
    template <typename T, class checker_>
    static constexpr auto impl_tuple_trait = false;

    template <typename... Ts, class checker_>
    static constexpr auto impl_tuple_trait<std::tuple<Ts...>, checker_> =
        (impl_props_trait<Ts, checker_> && ...);

public:
    /* Export widget type */
    using Widget = W;

    /* Export checker type */
    using Checker = checker;

    /* For check props */
    template <class T>
    static constexpr auto props_trait = impl_props_trait<std::remove_cvref_t<T>, checker>;

    /* For check tuple */
    template <class T>
    static constexpr auto tuple_trait = impl_tuple_trait<std::remove_cvref_t<T>, checker>;

public:
    // 铁血的，热血的，冷血的声明式构造接口
    explicit Declarative(auto&&... props) noexcept
        requires((props_trait<decltype(props)> || tuple_trait<decltype(props)>) && ...)
        : W {} {
        (apply(std::forward<decltype(props)>(props)), ...);
    }
    auto apply(auto&& tuple) noexcept -> void
        requires tuple_trait<decltype(tuple)>
    {
        std::apply([this](auto&&... args) { (apply(std::forward<decltype(args)>(args)), ...); },
            std::forward<decltype(tuple)>(tuple));
    }
    auto apply(auto&& prop) noexcept -> void
        requires props_trait<decltype(prop)>
    {
        std::forward<decltype(prop)>(prop).apply(*this);
    }
};

}
