#pragma once
#include "creeper-qt/utility/trait/widget.hh"
#include "creeper-qt/utility/wrapper/common.hh"

namespace creeper::layout::internal {

struct Layout { };

template <class T>
concept item_trait = widget_trait<T> || layout_trait<T>;

}
namespace creeper::layout::pro {

using Token = common::Token<internal::Layout>;

/// @brief
/// 布局项包装器，用于声明式地将 Widget 或 Layout 添加到布局中
///
/// @tparam T
/// 被包装的组件类型，需满足可转换为 QWidget* 或 QLayout*，不需
/// 要显式指定，由构造参数推倒
///
/// @note
/// Item 提供统一的接口用于在布局中插入控件或子布局，
/// 支持多种构造方式，包括直接传入指针或通过参数构造新对象。
/// 通过 LayoutMethod 可指定拉伸因子和对齐方式，
/// 在布局应用时自动选择 addWidget 或 addLayout，
/// 实现非侵入式的布局声明式封装。
///
/// 示例用途：
/// linear::pro::Item<Widget> {
///     { 0, Qt::AlignHCenter } // stretch, and alignment, optional
///     ...
/// };
///
template <internal::item_trait T> struct Item : Token {
    struct LayoutMethod {
        int stretch         = 0;
        Qt::Alignment align = {};
    } method;

    T* item_pointer = nullptr;

    explicit Item(const LayoutMethod& method, T* pointer) noexcept
        : item_pointer { pointer }
        , method { method } { }

    explicit Item(T* pointer) noexcept
        : item_pointer { pointer } { }

    explicit Item(const LayoutMethod& method, auto&&... args) noexcept
        requires std::constructible_from<T, decltype(args)...>
        : item_pointer { new T { std::forward<decltype(args)>(args)... } }
        , method(method) { }

    explicit Item(auto&&... args) noexcept
        requires std::constructible_from<T, decltype(args)...>
        : item_pointer { new T { std::forward<decltype(args)>(args)... } } { }

    void apply(container_trait auto& layout) const {
        if constexpr (std::is_convertible_v<T*, QWidget*>)
            layout.addWidget(item_pointer, method.stretch, method.align);
        if constexpr (std::is_convertible_v<T*, QLayout*>)
            layout.addLayout(item_pointer, method.stretch);
    }
};

// 传入一个方法用来辅助构造，在没有想要的接口时用这个吧
template <typename Lambda> struct Apply : Token {
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

}
