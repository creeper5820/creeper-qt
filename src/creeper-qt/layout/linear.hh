#pragma once

#include "creeper-qt/utility/api/scope/common.hh" // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/layout.hh" // IWYU pragma: keep
#include "creeper-qt/utility/trait/widget.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/utility/wrapper/forward_prop.hh"

#include <qboxlayout.h>
#include <qstackedlayout.h>

namespace creeper {

struct LinearPlacement {
    int stretch;
    Qt::Alignment align;
};

template <layout_trait T>
class BoxLayout : public T, public DSL {
public:
    using T::T;
    using Placement = LinearPlacement;

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
using Stretch     = ForwardProp<&QBoxLayout::addStretch>;
using SpacerItem  = ForwardProp<&QBoxLayout::addSpacerItem>;

/// @brief
/// 布局项包装器，用于声明式地将 Widget 或 Layout 添加到线性布局中
///
/// @tparam T
/// 被包装的组件类型，需满足可转换为 QWidget* 或 QLayout*，不需
/// 要显式指定，由构造参数推倒
///
/// @note
/// LinearItem 提供统一的接口用于在布局中插入控件或子布局，
/// 支持多种构造方式，包括直接传入指针或通过参数构造新对象。
/// 通过 BoxLayout::Placement（一般写作 Row::Placement / Col::Placement）
/// 可指定拉伸因子和对齐方式，并可由 Placement + 指针隐式得到。
///
/// 示例用途：
/// Row {
///     Row::Placement { 0, Qt::AlignHCenter } + new FilledButton { ... },
/// }
///
template <item_trait T>
struct LinearItem {
    LinearPlacement placement;
    T* item_pointer = nullptr;

    explicit LinearItem(LinearPlacement placement, T* pointer) noexcept
        : placement { placement }
        , item_pointer { pointer } { }

    template <typename... Args>
        requires std::constructible_from<T, Args...>
    explicit LinearItem(LinearPlacement placement, Args&&... args) noexcept
        : placement { placement }
        , item_pointer { new T { std::forward<Args>(args)... } } { }

    friend auto dsl_invoke(linear_trait auto& layout, const LinearItem& prop) -> void {
        if constexpr (widget_trait<T>)
            layout.addWidget(prop.item_pointer, prop.placement.stretch, prop.placement.align);
        if constexpr (layout_trait<T>) layout.addLayout(prop.item_pointer, prop.placement.stretch);
    }
};

using namespace api::scope::common;
using namespace api::scope::layout;
}

namespace creeper {

template <item_trait W>
auto operator+(W* item, LinearPlacement placement) -> linear::pro::LinearItem<W> {
    return linear::pro::LinearItem<W> { placement, item };
}

using Row = BoxLayout<QHBoxLayout>;
using Col = BoxLayout<QVBoxLayout>;

namespace row = linear; // NOLINT
namespace col = linear; // NOLINT

using HBoxLayout = Row;
using VBoxLayout = Col;

namespace h_box_layout = linear; // NOLINT
namespace v_box_layout = linear; // NOLINT
}
