#pragma once

#include <qobject.h>

#include <concepts>
#include <type_traits>
#include <utility>

namespace creeper::api::pro {

/// 通用点击事件
/// 要求组件有 clicked 信号
template <typename Callback>
struct Clickable {
    Callback callback;

    explicit Clickable(Callback cb) noexcept
        : callback { std::move(cb) } { }

    friend auto dsl_invoke(auto& widget, const Clickable& prop) -> void
        requires(std::invocable<Callback, decltype(widget)> || std::invocable<Callback>)
    {
        using widget_t = std::remove_cvref_t<decltype(widget)>;
        QObject::connect(&widget, &widget_t::clicked, [function = prop.callback, &widget] {
            if constexpr (std::invocable<Callback, decltype(widget)>) function(widget);
            else if constexpr (std::invocable<Callback>) function();
        });
    }
};

}
