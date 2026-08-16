#pragma once

#include <concepts>
#include <ranges>
#include <tuple>
#include <type_traits>
#include <utility>

namespace creeper {

/// TOKEN
struct TokenAncestor { };

template <class Tag>
struct Token : TokenAncestor {
    template <class T>
    static consteval auto inspect() -> bool {
        return std::derived_from<std::remove_cvref_t<T>, Token>;
    }
};

template <class T>
concept token_trait = std::derived_from<std::remove_cvref_t<T>, TokenAncestor>;

template <token_trait... Ts>
struct TokenOr : TokenAncestor {
    template <class T>
    static consteval auto inspect() -> bool {
        return (Ts::template inspect<T>() || ...);
    }
};

/// PROP
template <class Token, typename T, auto interface>
struct SetterProp : Token {
    T value;

    constexpr SetterProp() noexcept
        requires std::default_initializable<T>
    = default;

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

    template <typename... Args>
        requires std::constructible_from<T, Args&&...>
    constexpr explicit DerivedProp(Args&&... args)
        : T(std::forward<Args>(args)...) { }

    template <typename U>
        requires requires(T& t, U&& u) { t = std::forward<U>(u); }
    auto operator=(U&& value) -> DerivedProp& {
        T::operator=(std::forward<U>(value));
        return *this;
    }

    auto apply(this auto const& self, auto& widget) noexcept -> void {
        interface(widget, static_cast<const T&>(self));
    }
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
/// 该包装器支持将单个 prop、std::tuple 与 range 混合传入，不受顺序限制。内部通过
/// helper 检查 tuple 与 range 中的所有元素，递归调用 apply 将属性应用到 底层 Widget。
/// 利用 Token 延迟模板实例化，在 concept 层面约束属性类型， 从而避免因
/// 递归参数展开而产生的海量且难以定位的模板错误。
///
/// @tparam W
///   需要被包装的组件类型。
///
/// @tparam Token_
///   用于检查 prop 来源的 token。可以是组件自身 Token，也可以是 TokenOr 组合。
///
/// @note
/// 在不符合 token 要求的类型被传入时，会在 concept 约束阶段直接报错，
/// 提供简洁且精准的编译期错误提示，避免编译器自动展开大量构造函数
/// 导致的冗长错误栈，但编译期报错信息依旧不友好。
///
template <class Widget_, token_trait Token_>
struct Declarative : public Widget_ {
private:
    // 这里没法子塞一个 static_assert，无论如何这里都会被尝试错误地实例化
    template <typename T, class Token>
    static constexpr auto impl_props_trait = Token::template inspect<T>();

    // 使用 SFINAE 真是抱歉，没找到方便处理 tuple 的方法真是不好意思呢
    template <typename T, class Token>
    static constexpr auto impl_tuple_trait = false;

    template <typename... Ts, class Token>
    static constexpr auto impl_tuple_trait<std::tuple<Ts...>, Token> =
        (impl_props_trait<Ts, Token> && ...);

    // 同理 range：主模板 false，range 约束的偏特化检查 value_type
    template <typename T, class Token>
    static constexpr auto impl_range_trait = false;

    template <std::ranges::range R, class Token>
    static constexpr auto impl_range_trait<R, Token> =
        impl_props_trait<std::ranges::range_value_t<R>, Token>;

    /* For check props */
    template <class T>
    static constexpr auto props_trait = impl_props_trait<T, Token_>;

    template <class T>
    static constexpr auto range_trait = impl_range_trait<std::remove_cvref_t<T>, Token_>;

    /* For check tuple */
    template <class T>
    static constexpr auto tuple_trait = impl_tuple_trait<std::remove_cvref_t<T>, Token_>;

    template <class... Args>
    static constexpr auto mixed_trait =
        ((props_trait<Args> || tuple_trait<Args> || range_trait<Args>) && ...);

    // Error Message Helper

    template <typename T>
    static constexpr auto generate_prop_error_message() {
        static_assert(props_trait<T>,
            "<- 这里需要一个合法的声明式属性 | Expected a valid declarative prop ∑(￣□￣;)");
    }
    template <typename T>
    static constexpr auto generate_error_message(T&&) {
        generate_prop_error_message<T>();
    }
    template <typename... Ts>
    static constexpr auto generate_error_message(std::tuple<Ts...>&) {
        (generate_prop_error_message<Ts>(), ...);
    }
    template <typename... Ts>
    static constexpr auto generate_error_message(const std::tuple<Ts...>&) {
        (generate_prop_error_message<Ts>(), ...);
    }
    template <typename... Ts>
    static constexpr auto generate_error_message(std::tuple<Ts...>&&) {
        (generate_prop_error_message<Ts>(), ...);
    }

    template <std::ranges::range R>
    static constexpr auto generate_error_message(R&&) {
        generate_prop_error_message<std::ranges::range_value_t<R>>();
    }

public:
    using Widget = Widget_;
    using Token  = Token_;

    // 铁血的，热血的，冷血的声明式构造接口
    explicit Declarative(auto&&... props) noexcept
        requires mixed_trait<decltype(props)...>
        : Widget_ { } {
        (apply(std::forward<decltype(props)>(props)), ...);
    }

    /// @NOTE:
    ///  为了更友好的提示，但这个错误的 Fallback 会让
    ///  std::constructible_from 校验失效，避免依靠该
    ///  约束进行重载决议和编译期分发
    explicit Declarative(auto&&... props) noexcept {
        // 生成一些拟人的错误提示
        (generate_error_message(props), ...);
    }

    template <class T>
    auto apply(this auto& self, T&& tuple) noexcept -> void
        requires tuple_trait<T>
    {
        std::apply(
            [&self]<typename... Ts>(Ts&&... args) { (self.apply(std::forward<Ts>(args)), ...); },
            std::forward<T>(tuple));
    }

    template <class P>
    auto apply(this auto& self, P&& prop) noexcept -> void
        requires props_trait<P>
    {
        std::forward<P>(prop).apply(self);
    }

    template <class R>
    auto apply(this auto& self, R&& range) noexcept -> void
        requires range_trait<R> && (!props_trait<R>) && (!tuple_trait<R>)
    {
        for (auto&& prop : range)
            self.apply(prop);
    }
};

}
