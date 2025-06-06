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

namespace creeper {

template <typename Instance> struct InternalProperty {
    virtual void apply(Instance& _) const = 0;
};

namespace pro::common {

    template <typename Widget, class Token>
        requires requires(Widget widget) { widget.set_radius(double {}); }
    struct Radius final : Token {
        double radius;
        explicit Radius(double p) { radius = p; }
        void apply(Widget& self) const override { self.set_radius(radius); }
    };

    template <class Widget, class Token>
        requires requires(Widget widget) { widget.set_border(double {}); }
    struct Border final : Token {
        double border;
        explicit Border(double p) { border = p; }
        void apply(Widget& self) const override { self.set_border(border); }
    };

    template <class Widget, class Token>
        requires requires(Widget widget) { widget.setText(QString {}); }
    struct Text final : public QString, Token {
        using QString::QString;
        void apply(Widget& self) const override { self.setText(*this); }
    };

}

}
