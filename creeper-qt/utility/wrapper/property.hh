#pragma once

#include <tuple>
#include <utility>

/// @note 此处不使用 auto... 来作为构造参数，有体谅语法提示的考量，
///       为了更有效的编写期检查，还是谨慎地提供构造类型吧
///
/// @note 已废弃，大量侵入式的宏是不可控的
///
#define CREEPER_DEFINE_CONSTRUCTOR(CLASS, NAMESPACE)                                               \
public:                                                                                            \
    static_assert(false, "DO NOT USE MACRO METHOD, PLEASE USE DECLARATIVE WRAPPER");               \
    explicit CLASS(NAMESPACE::trait auto... properties) {                                          \
        (apply(std::forward<decltype(properties)>(properties)), ...);                              \
    }                                                                                              \
    template <NAMESPACE::trait... Args>                                                            \
    explicit CLASS(const std::tuple<Args...>& tuple, NAMESPACE::trait auto... properties) {        \
        apply(tuple), (apply(std::forward<decltype(properties)>(properties)), ...);                \
    }                                                                                              \
    template <NAMESPACE::trait... Args>                                                            \
    void apply(const std::tuple<Args...>& _) {                                                     \
        std::apply(                                                                                \
            [this](auto&&... args) { (apply(std::forward<decltype(args)>(args)), ...); }, _);      \
    }                                                                                              \
    void apply(const NAMESPACE::trait auto& property) { property.apply(*this); }

/// @note 少量样板代码的生成是可以接受的
///
#define CREEPER_DEFINE_CHECK(TRAIT)                                                                \
    struct checker final {                                                                         \
        template <class T>                                                                         \
        struct result {                                                                            \
            static constexpr auto v = TRAIT<T>;                                                    \
        };                                                                                         \
    };

namespace creeper {

template <class T>
concept checker_trait = requires {
    { T::template result<void>::v == false };
};

/// @brief
/// 声明式包装，非侵入式实现 Setter 的声明式化
///
/// 该包装器支持将 std::tuple 与单个 prop 混合传入，不受顺序限制。内部通过
/// helper 检查 tuple 中的所有元素，递归调用 apply 将属性应用到 底层 Widget。
/// 利用 CheckerT 延迟模板实例化，在 concept 层面约束属性类型， 从而避免因
/// 递归参数展开而产生的海量且难以定位的模板错误。
///
/// @tparam WidgetT
///   需要被包装的组件类型。
///
/// @tparam CheckerT
///   用于延迟模板实例化的“检查器”类型模板。典型形式如下：
/// struct checker final {
///     template <class T> struct result {
///         static constexpr auto v = concept<T>;
///     };
/// };
///
/// @note
/// 在不符合 CheckerT 要求的类型被传入时，会在 concept 约束阶段直接报错，
/// 提供简洁且精准的编译期错误提示，避免编译器自动展开大量构造函数
/// 导致的冗长错误栈，但编译期报错信息依旧不友好。
template <class WidgetT, class CheckerT>
    requires checker_trait<CheckerT>
struct Declarative : public WidgetT {
    /* Export widget type */
    using Widget = WidgetT;
    /* Export checker type */
    using Checker = CheckerT;

private:
    template <typename T, class Checker> /* For help check tuple of props */
    struct tuple_props_helper {
        static constexpr bool v = false;
    };
    template <typename... Ts, class Checker> /* For helpe check tuple of props */
    struct tuple_props_helper<std::tuple<Ts...>, Checker> {
        static constexpr bool v = (Checker::template result<std::remove_cvref_t<Ts>>::v && ...);
        static_assert(v,
            "\n[CREEPER-QT] 当你看到我，你便知道你的tuple里塞了错误的配置"
            "\n[CREEPER-QT] Your tuple contains the wrong configuration"
            "\n");
    };
    template <typename T, class Checker> //
    struct single_prop_helper {
        static constexpr bool v = Checker::template result<std::remove_cvref_t<T>>::v;
    };

public:
    template <class T> /* For check props */
    using props_trait = single_prop_helper<std::remove_cvref_t<T>, CheckerT>;

    template <class T> /* For check tuple */
    using tuple_trait = tuple_props_helper<std::remove_cvref_t<T>, CheckerT>;

    explicit Declarative(auto&&... props) noexcept
        requires((props_trait<decltype(props)>::v || tuple_trait<decltype(props)>::v) && ...)
        : WidgetT {} {
        (apply(std::forward<decltype(props)>(props)), ...);
    }

    auto apply(auto&& tuple) noexcept -> void
        requires tuple_trait<decltype(tuple)>::v
    {
        std::apply([this](auto&&... args) { (apply(std::forward<decltype(args)>(args)), ...); },
            std::forward<decltype(tuple)>(tuple));
    }

    auto apply(auto&& prop) noexcept -> void
        requires props_trait<decltype(prop)>::v
    {
        std::forward<decltype(prop)>(prop).apply(*this);
    }
};

}
