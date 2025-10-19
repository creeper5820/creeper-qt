#pragma once

#include "property.hh"

#include <qbitmap.h>
#include <qicon.h>
#include <qwidget.h>

namespace creeper::common {

template <typename Instance>
struct Token {
    void apply(auto& self) const {
        const auto self_name = typeid(self).name();
        const auto prop_name = typeid(this).name();
        // qDebug() << "Unimplemented" << prop_name << "is called by" << self_name;
    }
};

namespace pro {

    // 设置组建透明度
    template <class Token>
    using Opacity = SetterProp<Token, double, [](auto& self, double v) { self.set_opacity(v); }>;

    // 设置圆角（NXNY）
    template <class Token>
    using RadiusNxNy =
        SetterProp<Token, double, [](auto& self, double v) { self.set_radius_nx_ny(v); }>;

    // 设置圆角（PXPY）
    template <class Token>
    using RadiusPxPy =
        SetterProp<Token, double, [](auto& self, double v) { self.set_radius_px_py(v); }>;

    // 设置圆角（NXPY）
    template <class Token>
    using RadiusNxPy =
        SetterProp<Token, double, [](auto& self, double v) { self.set_radius_nx_py(v); }>;

    // 设置圆角（PXNY）
    template <class Token>
    using RadiusPxNy =
        SetterProp<Token, double, [](auto& self, double v) { self.set_radius_px_ny(v); }>;

    // 设置圆角（X方向）
    template <class Token>
    using RadiusX = SetterProp<Token, double, [](auto& self, double v) { self.set_radius_x(v); }>;

    // 设置圆角（Y方向）
    template <class Token>
    using RadiusY = SetterProp<Token, double, [](auto& self, double v) { self.set_radius_y(v); }>;

    // 设置通用圆角
    template <class Token>
    using Radius = SetterProp<Token, double, [](auto& self, double v) { self.set_radius(v); }>;

    // 通用边界宽度
    template <class Token>
    using BorderWidth =
        SetterProp<Token, double, [](auto& self, double v) { self.set_border_width(v); }>;

    // 通用边界颜色
    template <class Token>
    using BorderColor =
        SetterProp<Token, QColor, [](auto& self, const QColor& v) { self.set_border_color(v); }>;

    // 通用文字颜色
    template <class Token>
    using TextColor =
        SetterProp<Token, QColor, [](auto& self, const QColor& v) { self.set_text_color(v); }>;

    // 通用背景颜色
    template <class Token>
    using Background =
        SetterProp<Token, QColor, [](auto& self, const QColor& v) { self.set_background(v); }>;

    // 通用水波纹颜色
    template <class Token>
    using WaterColor =
        SetterProp<Token, QColor, [](auto& self, const QColor& v) { self.set_water_color(v); }>;

    // 通用禁止属性
    template <class Token>
    using Disabled = SetterProp<Token, bool, [](auto& self, bool v) { self.set_disabled(v); }>;

    // 通用 Checked 属性
    template <class Token>
    using Checked = SetterProp<Token, bool, [](auto& self, bool v) { self.set_checked(v); }>;

    // 通用文本属性
    template <class Token, auto setter>
    struct String : public QString, Token {
        using QString::QString;

        explicit String(const QString& text) noexcept
            : QString { text } { }
        explicit String(const std::string& text) noexcept
            : QString { QString::fromStdString(text) } { }

        auto operator=(const QString& text) noexcept {
            QString::operator=(text);
            return *this;
        }
        auto operator=(QString&& text) noexcept {
            QString::operator=(std::move(text));
            return *this;
        }

        void apply(auto& self) const
            requires requires { setter(self, *this); }
        {
            setter(self, *this);
        }
    };

    template <class Token>
    using Text = String<Token, [](auto& self, const auto& string) { self.setText(string); }>;

    // 通用指针绑定
    template <class Token, class Final>
    struct Bind : Token {
        Final*& widget;
        explicit Bind(Final*& p)
            : widget(p) { };
        void apply(Final& self) const { widget = &self; }
    };

    // 通用点击事件
    template <typename Callback, class Token>
    struct Clickable : Token {
        Callback callback;
        explicit Clickable(Callback callback) noexcept
            : callback { std::move(callback) } { }
        auto apply(auto& self) const noexcept -> void
            requires std::invocable<Callback, decltype(self)> || std::invocable<Callback>
        {
            using widget_t = std::remove_cvref_t<decltype(self)>;
            QObject::connect(&self, &widget_t::clicked, [function = callback, &self] {
                if constexpr (std::invocable<Callback, decltype(self)>) function(self);
                if constexpr (std::invocable<Callback>) function();
            });
        }
    };

    // 自定义信号回调注册

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
        concept tuple_invocable_trait = requires(F&& f, Tuple&& t) {
            std::apply(std::forward<F>(f), std::forward<Tuple>(t)); //
        };
    }

    template <typename F, class Token, auto signal>
    struct SignalInjection : Token {
        F f;

        using SignalArgs = typename internal::FunctionArgs<decltype(signal)>::type;

        explicit SignalInjection(F f) noexcept
            requires internal::tuple_invocable_trait<F, SignalArgs>
            : f { std::forward<decltype(f)>(f) } { }

        auto apply(auto& self) const noexcept { QObject::connect(&self, signal, f); }
    };

}
}
