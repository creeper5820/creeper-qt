#pragma once

#include "creeper-qt/utility/trait/widget.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/layout.hh"
#include "creeper-qt/utility/wrapper/property.hh"

#include <qboxlayout.h>

namespace creeper::linear::pro {

using Token = common::Token<QBoxLayout>;

struct SpacingItem : Token {
    int size;
    explicit SpacingItem(int p) { size = p; }
    void apply(QBoxLayout& self) const { self.addSpacing(size); }
};

struct Stretch : Token {
    int stretch;
    explicit Stretch(int p) { stretch = p; }
    void apply(QBoxLayout& self) const { self.addStretch(stretch); }
};

struct SpacerItem : Token {
    QSpacerItem* spacer_item;
    explicit SpacerItem(QSpacerItem* p) { spacer_item = p; }
    void apply(QBoxLayout& self) const { self.addSpacerItem(spacer_item); }
};

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
template <item_trait T>
struct Item : Token {
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

    void apply(linear_trait auto& layout) const {
        if constexpr (widget_trait<T>) layout.addWidget(item_pointer, method.stretch, method.align);
        if constexpr (layout_trait<T>) layout.addLayout(item_pointer, method.stretch);
    }
};

template <typename T>
concept trait = std::derived_from<T, Token>;

CREEPER_DEFINE_CHECKER(trait);
using namespace layout::pro;
}

namespace creeper {

template <class T>
using BoxLayout = Declarative<T, CheckerOr<linear::pro::checker, layout::pro::checker>>;

using Row = BoxLayout<QHBoxLayout>;
using Col = BoxLayout<QVBoxLayout>;

namespace row = linear;
namespace col = linear;

namespace internal {
    inline auto use_the_namespace_alias_to_eliminate_warnings() {
        std::ignore = row::pro::Token {};
        std::ignore = col::pro::Token {};
    }
}

}
