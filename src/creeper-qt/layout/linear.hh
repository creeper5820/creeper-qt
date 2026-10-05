#pragma once

#include "creeper-qt/utility/api/scope/common.hh"

#include "creeper-qt/utility/trait/widget.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/utility/wrapper/forward_prop.hh"
#include "creeper-qt/utility/wrapper/layout.hh"

#include <qboxlayout.h>
#include <qstackedlayout.h>

#include <type_traits>

namespace creeper {

template <layout_trait T>
class BoxLayout : public T, public DSL {
public:
    using T::T;

    explicit BoxLayout(auto&&... args) { construct_with(std::forward<decltype(args)>(args)...); }

private:
    template <widget_pointer_trait W>
    friend auto dsl_invoke(BoxLayout& self, W widget) {
        self.addWidget(widget, 0, { });
    }
    template <layout_pointer_trait L>
    friend auto dsl_invoke(BoxLayout& self, L layout) {
        self.addLayout(layout, 0);
    }
};

}

namespace creeper::linear::pro {

using SpacingItem = ForwardProp<&QBoxLayout::addSpacing>;

using Stretch = ForwardProp<&QBoxLayout::addStretch>;

using SpacerItem = ForwardProp<&QBoxLayout::addSpacerItem>;

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
struct Item {
    struct LayoutMethod {
        int stretch         = 0;
        Qt::Alignment align = { };
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

    template <typename... Args>
        requires std::constructible_from<T, Args...>
    explicit Item(Args&&... args) noexcept
        : item_pointer { new T { std::forward<Args>(args)... } } { }

    friend auto dsl_invoke(linear_trait auto& layout, const Item& prop) -> void {
        if constexpr (widget_trait<T>)
            layout.addWidget(prop.item_pointer, prop.method.stretch, prop.method.align);
        if constexpr (layout_trait<T>) layout.addLayout(prop.item_pointer, prop.method.stretch);
    }
};

using namespace common::pro;
using namespace api::scope::common;
using namespace layout::pro;
}
namespace creeper {

using Row = BoxLayout<QHBoxLayout>;
using Col = BoxLayout<QVBoxLayout>;

namespace row = linear;
namespace col = linear;

using HBoxLayout = Row;
using VBoxLayout = Col;

namespace h_box_layout = linear;
namespace v_box_layout = linear;
}
