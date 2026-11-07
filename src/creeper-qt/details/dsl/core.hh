#pragma once

#include <concepts>
#include <ranges>
#include <tuple>
#include <type_traits>
#include <utility>

namespace creeper {

template <class Arg, class Widget>
concept dsl_invocable = requires(Widget& w, Arg&& arg) {
    { dsl_invoke(w, std::forward<Arg>(arg)) } -> std::same_as<void>;
};

struct DSL {
private:
    // 这里没法子塞一个 static_assert，无论如何这里都会被尝试错误地实例化
    template <typename T, typename Self>
    static constexpr auto kImplPropsTrait = dsl_invocable<T, Self>;

    // 使用 SFINAE 真是抱歉，没找到方便处理 tuple 的方法真是不好意思呢
    template <typename T, typename Self>
    static constexpr auto kImplTupleTrait = false;

    template <typename... Ts, typename Self>
    static constexpr auto kImplTupleTrait<std::tuple<Ts...>, Self> =
        (kImplPropsTrait<Ts, Self> && ...);

    // 同理 range：主模板 false，range 约束的偏特化检查 value_type
    template <typename T, typename Self>
    static constexpr auto kImplRangeTrait = false;

    template <std::ranges::range R, typename Self>
    static constexpr auto kImplRangeTrait<R, Self> =
        kImplPropsTrait<std::ranges::range_value_t<R>, Self>;

    // Error Message Helper

    template <typename T, typename Self>
    static constexpr auto generate_prop_error_message() {
        static_assert(kImplPropsTrait<T, Self>,
            "<- 这里需要一个合法的声明式属性 | Expected a valid declarative prop ∑(￣□□￣;)");
    }

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
    template <class T>
    auto use(this auto& self, T&& tuple) noexcept -> void
        requires kImplTupleTrait<std::remove_cvref_t<T>, decltype(self)>
    {
        std::apply(
            [&self]<typename... Ts>(Ts&&... args) { (self.use(std::forward<Ts>(args)), ...); },
            std::forward<T>(tuple));
    }
    template <std::ranges::range R>
    auto use(this auto& self, R&& range) noexcept -> void
        requires kImplRangeTrait<R, decltype(self)> && (!kImplPropsTrait<R, decltype(self)>)
        && (!kImplTupleTrait<std::remove_cvref_t<R>, decltype(self)>)
    {
        for (auto&& item : std::forward<R>(range)) {
            self.use(std::forward<decltype(item)>(item));
        }
    }
    template <class T>
    auto use(this auto& self, T&& prop) noexcept -> void
        requires kImplPropsTrait<T, decltype(self)>
    {
        dsl_invoke(self, std::forward<T>(prop));
    }

protected:
    /// @brief
    /// 该函数检查所有传入参数是否为合法的声明式属性，如果合法则应用，
    /// 否则触发错误诊断。错误诊断会在 LSP 和编译期提供精确的错误定位。
    ///
    /// 使用示例：
    /// @code
    ///     template<typename... Props>
    ///     explicit MyWidget(Props&&... props) noexcept {
    ///         construct_with(std::forward<Props>(props)...);
    ///     }
    /// @endcode
    template <class... Args>
    auto construct_with(this auto& self, Args&&... args) noexcept -> void {
        using Self = decltype(self);

        constexpr auto kCheck = []<typename T> noexcept {
            using Tuple = std::remove_cvref_t<T>;
            return kImplPropsTrait<T, Self> || kImplTupleTrait<Tuple, Self>
                || kImplRangeTrait<T, Self>;
        };
        if constexpr ((kCheck.template operator()<Args>() && ...)) {
            (self.use(std::forward<Args>(args)), ...);
        } else {
            // 生成一些拟人的错误提示
            (generate_error_for_construct<Self>(args), ...);
        }
    }
};

} // namespace creeper
