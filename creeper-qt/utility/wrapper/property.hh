#pragma once

#include <qbitmap.h>
#include <qicon.h>
#include <qwidget.h>

/// @note 此处不使用 auto... 来作为构造参数，有体谅语法提示的考量，
///         为了更有效的编写期检查，还是谨慎地提供构造类型吧
#define CREEPER_DEFINE_CONSTROCTOR(CLASS, NAMESPACE)                                               \
public:                                                                                            \
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

namespace creeper::common {

namespace pro {

    // 通用半径长度
    template <typename Widget, class Token>
        requires requires(Widget widget) { widget.set_radius(double {}); }
    struct RadiusX final : Token {
        double radius;
        explicit RadiusX(double p) { radius = p; }
        void apply(Widget& self) const override { self.set_radius_x(radius); }
    };
    template <typename Widget, class Token>
        requires requires(Widget widget) { widget.set_radius(double {}); }
    struct RadiusY final : Token {
        double radius;
        explicit RadiusY(double p) { radius = p; }
        void apply(Widget& self) const override { self.set_radius_y(radius); }
    };
    template <typename Widget, class Token>
        requires requires(Widget widget) { widget.set_radius(double {}); }
    struct Radius final : Token {
        double radius;
        explicit Radius(double p) { radius = p; }
        void apply(Widget& self) const override { self.set_radius(radius); }
    };

    // 通用边界宽度
    template <class Widget, class Token>
        requires requires(Widget widget) { widget.set_border_width(double {}); }
    struct BorderWidth final : Token {
        double border;
        explicit BorderWidth(double p) { border = p; }
        void apply(Widget& self) const override { self.set_border_width(border); }
    };

    // 通用边界颜色
    template <class Widget, class Token>
        requires requires(Widget widget) { widget.set_border_color(QColor {}); }
    struct BorderColor final : public QColor, Token {
        using QColor::QColor;
        void apply(Widget& self) const override { self.set_border_color(*this); }
    };

    // 通用文本属性
    template <class Widget, class Token>
        requires requires(Widget widget) { widget.setText(QString {}); }
    struct Text final : public QString, Token {
        using QString::QString;
        void apply(Widget& self) const override { self.setText(*this); }
    };

    // 通用文字颜色
    template <class Widget, class Token>
        requires requires(Widget widget) { widget.set_text_color(QColor {}); }
    struct TextColor final : public QColor, Token {
        using QColor::QColor;
        void apply(Widget& self) const override { self.set_text_color(*this); }
    };

    // 通用背景颜色
    template <class Widget, class Token>
        requires requires(Widget widget) { widget.set_background(QColor {}); }
    struct Background final : public QColor, Token {
        using QColor::QColor;
        void apply(Widget& self) const override { self.set_background(*this); }
    };

    // 通用水波纹颜色
    template <class Widget, class Token>
        requires requires(Widget widget) { widget.set_water_color(QColor {}); }
    struct WaterColor final : public QColor, Token {
        using QColor::QColor;
        void apply(Widget& self) const override { self.set_water_color(*this); }
    };

}

template <typename Instance> struct InternalProperty {
    virtual void apply(Instance& _) const = 0;
};

}
