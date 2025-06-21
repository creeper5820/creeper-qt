#pragma once

#include <qbitmap.h>
#include <qdebug.h>
#include <qicon.h>
#include <qwidget.h>

namespace creeper::common {
template <typename Instance> struct Token {
    void apply(auto& self) const {
        const auto self_name = typeid(self).name();
        const auto prop_name = typeid(this).name();
        qDebug() << "Unimplemented" << prop_name << "is called by" << self_name;
    }
};

namespace pro {

    // 特殊属性，用于绑定自身指针
    template <class Token, class Final> struct Bind final : Token {
        Final*& widget;
        explicit Bind(Final*& p)
            : widget(p) { };
        void apply(Final& self) const { widget = &self; }
    };

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
        explicit Text(const QString& text)
            : QString { text } { }
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
            requires requires { self.set_water_color(QColor {}); }
        {
            self.set_water_color(*this);
        }
    };

    // 通用禁止属性
    template <class Token> struct Disabled final : Token {
        bool disabled;
        explicit Disabled(bool p) { disabled = p; }
        void apply(auto& self) const
            requires requires { self.set_disabled(bool {}); }
        {
            self.set_disabled(disabled);
        }
    };

    // 通用 Checked 属性
    template <class Token> struct Checked final : Token {
        bool checked;
        explicit Checked(bool p) { checked = p; }
        void apply(auto& self) const
            requires requires { self.set_checked(bool {}); }
        {
            self.set_checked(checked);
        }
    };

}
}
