#pragma once

#include <qbitmap.h>
#include <qdebug.h>
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

template <typename Instance> struct Token {
    void apply(auto& self) const {
        const auto self_name = typeid(self).name();
        const auto prop_name = typeid(this).name();
        qDebug() << "Unimplemented" << prop_name << "is called by" << self_name;
    }
};

namespace pro {

    // 通用半径长度
    template <class Token> struct RadiusX final : Token {
        double radius;
        explicit RadiusX(double p) { radius = p; }
        void apply(auto& self) const
            requires requires { self.set_radius_x(radius); }
        {
            self.set_radius_x(radius);
        }
    };
    template <class Token> struct RadiusY final : Token {
        double radius;
        explicit RadiusY(double p) { radius = p; }
        void apply(auto& self) const
            requires requires { self.set_radius_y(radius); }
        {
            self.set_radius_y(radius);
        }
    };
    template <class Token> struct Radius final : Token {
        double radius;
        explicit Radius(double p) { radius = p; }
        void apply(auto& self) const
            requires requires { self.set_radius(radius); }
        {
            self.set_radius(radius);
        }
    };

    // 通用边界宽度
    template <class Token> struct BorderWidth final : Token {
        double border;
        explicit BorderWidth(double p) { border = p; }
        void apply(auto& self) const
            requires requires { self.set_border_width(border); }
        {
            self.set_border_width(border);
        }
    };

    // 通用边界颜色
    template <class Token> struct BorderColor final : public QColor, Token {
        using QColor::QColor;
        explicit BorderColor(const QColor& color)
            : QColor(color) { }
        void apply(auto& self) const
            requires requires { self.set_border_color(*this); }
        {
            self.set_border_color(*this);
        }
    };

    // 通用文本属性
    template <class Token> struct Text final : public QString, Token {
        using QString::QString;
        void apply(auto& self) const
            requires requires { self.setText(*this); }
        {
            self.setText(*this);
        }
    };

    // 通用文字颜色
    template <class Token> struct TextColor final : public QColor, Token {
        using QColor::QColor;
        explicit TextColor(const QColor& color)
            : QColor(color) { }
        void apply(auto& self) const
            requires requires { self.set_text_color(*this); }
        {
            self.set_text_color(*this);
        }
    };

    // 通用背景颜色
    template <class Token> struct Background final : public QColor, Token {
        using QColor::QColor;
        explicit Background(const QColor& color)
            : QColor(color) { }
        void apply(auto& self) const
            requires requires { self.set_background(*this); }
        {
            self.set_background(*this);
        }
    };

    // 通用水波纹颜色
    template <class Token> struct WaterColor final : public QColor, Token {
        using QColor::QColor;
        explicit WaterColor(const QColor& color)
            : QColor(color) { }
        void apply(auto& self) const
            requires requires { self.set_water_color(*this); }
        {
            self.set_water_color(*this);
        }
    };

}

}
