#pragma once

#include <concepts>
#include <qcolor.h>
#include <qobject.h>
#include <qstring.h>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

namespace creeper::common::pro {

// ============================================================================
// 基础属性 - 通过方法名约定实现通用性
// ============================================================================

/// 设置组件透明度
/// 要求组件实现：void setOpacity(double)
struct Opacity {
    double value;

    constexpr explicit Opacity(double v) noexcept
        : value { v } { }

    friend auto dsl_invoke(auto& widget, const Opacity& prop) -> void {
        widget.setOpacity(prop.value);
    }
};

/// 设置圆角（NXNY）
/// 要求组件实现：void setRadiusNxNy(double)
struct RadiusNxNy {
    double value;

    constexpr explicit RadiusNxNy(double v) noexcept
        : value { v } { }

    friend auto dsl_invoke(auto& widget, const RadiusNxNy& prop) -> void {
        widget.setRadiusNxNy(prop.value);
    }
};

/// 设置圆角（PXPY）
/// 要求组件实现：void setRadiusPxPy(double)
struct RadiusPxPy {
    double value;

    constexpr explicit RadiusPxPy(double v) noexcept
        : value { v } { }

    friend auto dsl_invoke(auto& widget, const RadiusPxPy& prop) -> void {
        widget.setRadiusPxPy(prop.value);
    }
};

/// 设置圆角（NXPY）
/// 要求组件实现：void setRadiusNxPy(double)
struct RadiusNxPy {
    double value;

    constexpr explicit RadiusNxPy(double v) noexcept
        : value { v } { }

    friend auto dsl_invoke(auto& widget, const RadiusNxPy& prop) -> void {
        widget.setRadiusNxPy(prop.value);
    }
};

/// 设置圆角（PXNY）
/// 要求组件实现：void setRadiusPxNy(double)
struct RadiusPxNy {
    double value;

    constexpr explicit RadiusPxNy(double v) noexcept
        : value { v } { }

    friend auto dsl_invoke(auto& widget, const RadiusPxNy& prop) -> void {
        widget.setRadiusPxNy(prop.value);
    }
};

/// 设置圆角（X方向）
/// 要求组件实现：void setRadiusX(double)
struct RadiusX {
    double value;

    constexpr explicit RadiusX(double v) noexcept
        : value { v } { }

    friend auto dsl_invoke(auto& widget, const RadiusX& prop) -> void {
        widget.setRadiusX(prop.value);
    }
};

/// 设置圆角（Y方向）
/// 要求组件实现：void setRadiusY(double)
struct RadiusY {
    double value;

    constexpr explicit RadiusY(double v) noexcept
        : value { v } { }

    friend auto dsl_invoke(auto& widget, const RadiusY& prop) -> void {
        widget.setRadiusY(prop.value);
    }
};

/// 设置通用圆角
/// 要求组件实现：void setRadius(double)
struct Radius {
    double value;

    constexpr explicit Radius(double v) noexcept
        : value { v } { }

    friend auto dsl_invoke(auto& widget, const Radius& prop) -> void {
        widget.setRadius(prop.value);
    }
};

/// 通用边界宽度
/// 要求组件实现：void setBorderWidth(double)
struct BorderWidth {
    double value;

    constexpr explicit BorderWidth(double v) noexcept
        : value { v } { }

    friend auto dsl_invoke(auto& widget, const BorderWidth& prop) -> void {
        widget.setBorderWidth(prop.value);
    }
};

/// 通用边界颜色
/// 要求组件实现：void setBorderColor(const QColor&)
struct BorderColor {
    QColor value;

    explicit BorderColor(QColor v) noexcept
        : value { std::move(v) } { }

    friend auto dsl_invoke(auto& widget, const BorderColor& prop) -> void {
        widget.setBorderColor(prop.value);
    }
};

/// 通用文字颜色
/// 要求组件实现：void setTextColor(const QColor&)
struct TextColor {
    QColor value;

    explicit TextColor(QColor v) noexcept
        : value { std::move(v) } { }

    friend auto dsl_invoke(auto& widget, const TextColor& prop) -> void {
        widget.setTextColor(prop.value);
    }
};

/// 通用背景颜色
/// 要求组件实现：void setBackground(const QColor&)
struct Background {
    QColor value;

    explicit Background(QColor v) noexcept
        : value { std::move(v) } { }

    friend auto dsl_invoke(auto& widget, const Background& prop) -> void {
        widget.setBackground(prop.value);
    }
};

/// 通用水波纹颜色
/// 要求组件实现：void setWaterColor(const QColor&)
struct WaterColor {
    QColor value;

    explicit WaterColor(QColor v) noexcept
        : value { std::move(v) } { }

    friend auto dsl_invoke(auto& widget, const WaterColor& prop) -> void {
        widget.setWaterColor(prop.value);
    }
};

/// 通用禁止属性
/// 要求组件实现：void setDisabled(bool)
struct Disabled {
    bool value;

    constexpr explicit Disabled(bool v) noexcept
        : value { v } { }

    friend auto dsl_invoke(auto& widget, const Disabled& prop) -> void {
        widget.setDisabled(prop.value);
    }
};

/// 通用 Checked 属性
/// 要求组件实现：void setChecked(bool)
struct Checked {
    bool value;

    constexpr explicit Checked(bool v) noexcept
        : value { v } { }

    friend auto dsl_invoke(auto& widget, const Checked& prop) -> void {
        widget.setChecked(prop.value);
    }
};

// ============================================================================
// 文本属性 - 继承 QString 提供完整字符串功能
// ============================================================================

/// 通用文本属性（可定制 setter）
/// 默认调用 widget.setText()
template <auto setter = [](auto& self, const auto& text) { self.setText(text); }>
struct String : public QString {
    using QString::QString;

    explicit String(const QString& text) noexcept
        : QString { text } { }

    explicit String(const std::string& text) noexcept
        : QString { QString::fromStdString(text) } { }

    auto operator=(const QString& text) noexcept -> String& {
        QString::operator=(text);
        return *this;
    }

    auto operator=(QString&& text) noexcept -> String& {
        QString::operator=(std::move(text));
        return *this;
    }

    friend auto dsl_invoke(auto& widget, const String& prop) -> void
        requires requires { setter(widget, static_cast<const QString&>(prop)); }
    {
        setter(widget, static_cast<const QString&>(prop));
    }
};

/// 标准 Text 属性
/// 要求组件实现：void setText(const QString&)
using Text = String<>;

// ============================================================================
// 指针绑定 - 将构造的组件指针绑定到外部变量
// ============================================================================

/// 通用指针绑定
/// 用于在构造组件时获取其指针
template <class Final>
struct Bind {
    Final*& widget;

    explicit Bind(Final*& p) noexcept
        : widget(p) { }

    friend auto dsl_invoke(Final& self, const Bind& prop) -> void { prop.widget = &self; }
};

// ============================================================================
// 事件回调 - 点击事件
// ============================================================================

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

// ============================================================================
// 信号注入 - 自定义信号回调
// ============================================================================

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

/// 信号注入 - 连接任意信号到回调
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

} // namespace creeper::common::pro
