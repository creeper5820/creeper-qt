#pragma once

#include <concepts>
#include <ranges>
#include <tuple>
#include <type_traits>
#include <utility>

namespace creeper {

/// @brief
/// DSL 定制点，通过 ADL 查找对应的 dsl_invoke 实现
///
/// @tparam Arg
///   传入的 prop 类型
///
/// @tparam Widget
///   目标组件类型
///
/// @note
/// 该 concept 用于检查某个类型是否可以作为指定组件的声明式属性。
/// 不同于基于继承的 Token 系统，DSL 使用 ADL (Argument-Dependent Lookup)
/// 查找 dsl_invoke 函数，允许非侵入式地为任意类型添加声明式能力。
///
template <class Arg, class Widget>
concept dsl_invocable = requires(Widget& w, Arg&& arg) {
    { dsl_invoke(w, std::forward<Arg>(arg)) } -> std::same_as<void>;
};

/// @brief
/// 声明式基类，为组件提供声明式构造能力
///
/// 该基类通过 CRTP 的简化版本（利用 C++23 的 deducing this）为派生类提供
/// 声明式属性的应用接口。组件只需继承 DSL 并在构造函数中调用 construct_with，
/// 即可获得完整的声明式构造能力，支持单个 prop、std::tuple 与 range 的混合传入。
///
/// @note
/// 与原有的 Declarative 包装相比，DSL 基类采用侵入式设计，组件直接继承 DSL，
/// 无需额外的包装层和 Token 系统。错误诊断通过 dsl_invocable concept 在编译期
/// 提供精准的错误提示，tuple 中的错误元素会被逐一展开，避免冗长的模板错误栈。
///
/// 使用示例：
/// @code
///     class MyWidget : public QWidget, public DSL {
///     public:
///         explicit MyWidget(auto&&... props) noexcept
///             : QWidget() {
///             construct_with(std::forward<decltype(props)>(props)...);
///         }
///     };
/// @endcode
///
struct DSL {
private:
    // 这里没法子塞一个 static_assert，无论如何这里都会被尝试错误地实例化
    template <typename T, typename Self>
    static constexpr auto impl_props_trait = dsl_invocable<T, Self>;

    // 使用 SFINAE 真是抱歉，没找到方便处理 tuple 的方法真是不好意思呢
    template <typename T, typename Self>
    static constexpr auto impl_tuple_trait = false;

    template <typename... Ts, typename Self>
    static constexpr auto impl_tuple_trait<std::tuple<Ts...>, Self> =
        (impl_props_trait<Ts, Self> && ...);

    // 同理 range：主模板 false，range 约束的偏特化检查 value_type
    template <typename T, typename Self>
    static constexpr auto impl_range_trait = false;

    template <std::ranges::range R, typename Self>
    static constexpr auto impl_range_trait<R, Self> =
        impl_props_trait<std::ranges::range_value_t<R>, Self>;

    // Error Message Helper

    template <typename T, typename Self>
    static constexpr auto generate_prop_error_message() {
        static_assert(impl_props_trait<T, Self>,
            "<- 这里需要一个合法的声明式属性 | Expected a valid declarative prop ∑(￣□□￣;)");
    }

    /// @brief
    /// 用于 construct_with 的错误消息生成辅助函数
    ///
    /// @note
    /// 该函数族的关键在于参数类型从函数参数推导，而非显式模板参数。
    /// 当传入 std::tuple 时，会匹配到 tuple 重载并展开每个元素进行诊断，
    /// 从而在 LSP 和编译器错误中精确定位 tuple 内部的非法 prop。
    ///
    template <typename Self, typename T>
    static constexpr auto generate_error_for_construct(T&&) {
        generate_prop_error_message<std::remove_cvref_t<T>, Self>();
    }

    template <typename Self, typename... Ts>
    static constexpr auto generate_error_for_construct(std::tuple<Ts...>&) {
        (generate_prop_error_message<Ts, Self>(), ...);
    }

    template <typename Self, typename... Ts>
    static constexpr auto generate_error_for_construct(const std::tuple<Ts...>&) {
        (generate_prop_error_message<Ts, Self>(), ...);
    }

    template <typename Self, typename... Ts>
    static constexpr auto generate_error_for_construct(std::tuple<Ts...>&&) {
        (generate_prop_error_message<Ts, Self>(), ...);
    }

    template <typename Self, std::ranges::range R>
    static constexpr auto generate_error_for_construct(R&&) {
        generate_prop_error_message<std::ranges::range_value_t<R>, Self>();
    }

public:
    /// @brief
    /// 应用 tuple 中的所有属性
    ///
    /// @note
    /// 递归展开 tuple，对每个元素调用 use
    ///
    template <class T>
    auto use(this auto& self, T&& tuple) noexcept -> void
        requires impl_tuple_trait<std::remove_cvref_t<T>, decltype(self)>
    {
        std::apply(
            [&self]<typename... Ts>(Ts&&... args) { (self.use(std::forward<Ts>(args)), ...); },
            std::forward<T>(tuple));
    }

    /// @brief
    /// 应用 range 中的所有属性
    ///
    /// @note
    /// 遍历 range，对每个元素调用 use
    ///
    template <std::ranges::range R>
    auto use(this auto& self, R&& range) noexcept -> void
        requires impl_range_trait<R, decltype(self)> && (!impl_props_trait<R, decltype(self)>)
        && (!impl_tuple_trait<std::remove_cvref_t<R>, decltype(self)>)
    {
        for (auto&& item : std::forward<R>(range)) {
            self.use(std::forward<decltype(item)>(item));
        }
    }

    /// @brief
    /// 应用单个属性
    ///
    /// @note
    /// 通过 ADL 查找 dsl_invoke 并调用
    ///
    template <class T>
    auto use(this auto& self, T&& prop) noexcept -> void
        requires impl_props_trait<T, decltype(self)>
    {
        dsl_invoke(self, std::forward<T>(prop));
    }

protected:
    /// @brief
    /// 声明式构造辅助函数，供子类构造函数调用
    ///
    /// @note
    /// 该函数检查所有传入参数是否为合法的声明式属性，如果合法则应用，
    /// 否则触发错误诊断。错误诊断会在 LSP 和编译期提供精确的错误定位。
    ///
    /// 使用示例：
    /// @code
    ///     explicit MyWidget(auto&&... props) noexcept {
    ///         construct_with(std::forward<decltype(props)>(props)...);
    ///     }
    /// @endcode
    ///
    template <class... Args>
    auto construct_with(this auto& self, Args&&... args) noexcept -> void {
        using Self = decltype(self);

        if constexpr (((impl_props_trait<Args, Self>
                           || impl_tuple_trait<std::remove_cvref_t<Args>, Self>
                           || impl_range_trait<Args, Self>)
                          && ...)) {
            (self.use(std::forward<Args>(args)), ...);
        } else {
            // 生成一些拟人的错误提示
            (generate_error_for_construct<Self>(args), ...);
        }
    }
};

} // namespace creeper
