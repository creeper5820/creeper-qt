#pragma once

#include <tuple>
#include <type_traits>

/// @note 此处不使用 auto... 来作为构造参数，有体谅语法提示的考量，
///         为了更有效的编写期检查，还是谨慎地提供构造类型吧
#define CREEPER_DEFINE_CONSTRUCTOR(CLASS, NAMESPACE)                                               \
public:                                                                                            \
    static_assert(false, "DO NOT USE MACRO METHOD, PLEASE USE DECLARATIVE WRAPPER");               \
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

namespace creeper {

/// @brief
/// 声明式包装，非侵入式实现 Setter 的声明式化
///
/// @tparam _widget
/// 需要包装的组件
///
/// @tparam _checker
/// 套两层类型可以延迟模板的实例化，将类型推迟到调用函数时
/// 这里使用实际绝对不会使用的类型 void 作为检查
/// 一般写成如下形式:
/// struct checker {
///     template <class T> struct result {
///         static constexpr auto v = false;
///     };
///     template <property_concept T> struct result<T> {
///         static constexpr auto v = true;
///     };
/// };
/// @note
/// 使用该声明式包装器时，模板参数的递归实例化会导致编译器自动生成一系列参数极多的构造函数，
/// 例如：
///
/// In Declarative<Impl, checker>
/// template <> explicit Declarative<
///         creeper::util::theme::pro::ThemeManager,
///         creeper::card::pro::Level,
///         creeper::widget::pro::WindowFlag,
///         creeper::common::pro::Radius<...>,
///         creeper::widget::pro::Layout<...>,
///         >
///
/// 一旦中间某一层模板参数类型有误，
/// 编译器会将错误信息逐层上报，最终导致巨量且难以定位的模板报错。
/// 因此，在 concept 层面进行类型约束是必要之举，而使用 checker 进行 concept
/// 注入可以大幅简化开发流程， 以便在编写代码时即可获得清晰的类型错误提示，
/// 避免编译期出现冗长难读的错误栈。
///
template <class _widget, class _checker>
    requires requires {
        { _checker::template result<void>::v };
    }
struct Declarative : public _widget {

    template <class T> using require = _checker::template result<std::remove_cvref_t<T>>;

    explicit Declarative(const auto&... properties) noexcept
        requires(require<decltype(properties)>::v && ...)
        : _widget {} {
        (apply(properties), ...);
    }

    template <class... _args_1, class... _args_2>
    explicit Declarative(const std::tuple<_args_1...>& tuple, const _args_2&... properties) noexcept
        requires(require<_args_1>::v && ...) && (require<_args_2>::v && ...)
        : _widget {} {
        apply(tuple), (apply(properties), ...);
    }

    auto apply(const auto& property) noexcept -> void
        requires require<decltype(property)>::v
    {
        property.apply(*this);
    }

    template <class... _args>
    auto apply(const std::tuple<_args...>& tuple) noexcept -> void
        requires(require<_args>::v && ...)
    {
        std::apply([this](const auto&... args) { (apply(args), ...); }, tuple);
    }
};

}
