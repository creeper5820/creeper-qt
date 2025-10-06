#pragma once
#include "creeper-qt/utility/qt_wrapper/margin_setter.hh"
#include "creeper-qt/utility/trait/widget.hh"
#include "creeper-qt/utility/wrapper/common.hh"

namespace creeper::layout::pro {

struct Layout { };
using Token = common::Token<Layout>;

using ContentsMargin = SetterProp<Token, QMargins,
    [](auto& self, const auto& margins) { self.setContentsMargins(margins); }>;

using Alignment = SetterProp<Token, Qt::Alignment,
    [](auto& self, const auto& alignment) { self.setAlignment(alignment); }>;

using Spacing =
    SetterProp<Token, int, [](auto& self, const auto& spacing) { self.setSpacing(spacing); }>;

using Margin = SetterProp<Token, int, qt::margin_setter>;

template <widget_trait T>
struct Widget : Token {

    T* item_pointer = nullptr;

    explicit Widget(T* pointer) noexcept
        : item_pointer { pointer } { }

    explicit Widget(auto&&... args) noexcept
        requires std::constructible_from<T, decltype(args)...>
        : item_pointer { new T { std::forward<decltype(args)>(args)... } } { }

    auto apply(auto& layout) const { layout.addWidget(item_pointer); }
};

// 传入一个方法用来辅助构造，在没有想要的接口时用这个吧
template <typename Lambda>
struct Apply : Token {
    Lambda lambda;
    explicit Apply(Lambda lambda) noexcept
        : lambda { lambda } { }
    auto apply(auto& self) const noexcept -> void {
        if constexpr (std::invocable<Lambda>) lambda();
        if constexpr (std::invocable<Lambda, decltype(self)>) lambda(self);
    }
};

template <class T>
concept trait = std::derived_from<T, Token>;

CREEPER_DEFINE_CHECKER(trait);
}
