#pragma once

#include <qobject.h>

#include <concepts>
#include <tuple>
#include <type_traits>
#include <utility>

namespace creeper::api::helper {

namespace internal {

    template <typename T>
    struct FunctionArgs;

    template <class C, class R, class... Args>
    struct FunctionArgs<auto (C::*)(Args...)->R> {
        using type = std::tuple<Args...>;
    };

    template <class C, class R, class... Args>
    struct FunctionArgs<auto (C::*)(Args...) const->R> {
        using type = std::tuple<Args...>;
    };

    template <typename F, typename Tuple>
    concept tuple_invocable_trait =
        requires(F&& f, Tuple&& t) { std::apply(std::forward<F>(f), std::forward<Tuple>(t)); };

} // namespace internal

/// @brief 信号注入 - 连接任意信号到回调
/// @tparam F 回调函数类型
/// @tparam signal 信号成员指针
template <typename F, auto signal>
struct SignalInjection {
    F f;

    using SignalArgs = typename internal::FunctionArgs<decltype(signal)>::type;

    explicit SignalInjection(F func) noexcept
        requires internal::tuple_invocable_trait<F, SignalArgs>
        : f { std::forward<F>(func) } { }

    friend auto dsl_invoke(auto& widget, const SignalInjection& prop) -> void {
        QObject::connect(&widget, signal, prop.f);
    }
};

}
