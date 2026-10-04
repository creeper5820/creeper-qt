#pragma once
#include "creeper-qt/utility/qt_wrapper/margin-setter.hh"
#include "creeper-qt/utility/trait/widget.hh"
#include "creeper-qt/utility/wrapper/common.hh"

namespace creeper::layout::pro {

struct ContentsMargin : public QMargins {
    using QMargins::QMargins;
    ContentsMargin(const QMargins& margins)
        : QMargins(margins) { }

    friend auto dsl_invoke(auto& layout, const ContentsMargin& prop) -> void {
        layout.setContentsMargins(static_cast<const QMargins&>(prop));
    }
};

struct Alignment {
    Qt::Alignment value;

    explicit Alignment(Qt::Alignment v) noexcept
        : value { v } { }

    friend auto dsl_invoke(auto& layout, const Alignment& prop) -> void {
        layout.setAlignment(prop.value);
    }
};

struct Spacing {
    int value;

    constexpr explicit Spacing(int v) noexcept
        : value { v } { }

    friend auto dsl_invoke(auto& layout, const Spacing& prop) -> void {
        layout.setSpacing(prop.value);
    }
};

struct Margin {
    int value;

    constexpr explicit Margin(int v) noexcept
        : value { v } { }

    friend auto dsl_invoke(auto& layout, const Margin& prop) -> void {
        qt::margin_setter(layout, prop.value);
    }
};

template <widget_trait T>
struct Widget {

    T* item_pointer = nullptr;

    explicit Widget(T* pointer) noexcept
        : item_pointer { pointer } { }

    explicit Widget(auto&&... args) noexcept
        requires std::constructible_from<T, decltype(args)...>
        : item_pointer { new T { std::forward<decltype(args)>(args)... } } { }

    friend auto dsl_invoke(auto& layout, const Widget& prop) -> void {
        layout.addWidget(prop.item_pointer);
    }
};

// 传入一个方法用来辅助构造，在没有想要的接口时用这个吧
template <typename Lambda>
struct Apply {
    Lambda lambda;
    explicit Apply(Lambda lambda) noexcept
        : lambda { lambda } { }

    friend auto dsl_invoke(auto& self, const Apply& prop) -> void {
        if constexpr (std::invocable<Lambda>) prop.lambda();
        if constexpr (std::invocable<Lambda, decltype(self)>) prop.lambda(self);
    }
};
}
