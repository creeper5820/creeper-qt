#pragma once

#include <qwidget.h>

#include <concepts>
#include <utility>

namespace creeper::api::pro {

/**
 * @brief 设置 parent
 *
 * 调用 QWidget::setParent。
 */
struct Parent {
    QWidget* value;

    explicit Parent(QWidget* v) noexcept
        : value { v } { }

    friend auto dsl_invoke(QWidget& widget, const Parent& prop) -> void {
        widget.setParent(prop.value);
    }
};

/**
 * @brief 委托构造子组件并将其 parent 设为当前组件
 *
 * 适用于弹窗、菜单等需要 QObject parent 提供生命周期与定位上下文、
 * 但不进入布局的组件，例如把 DropdownMenu 声明式地锚定到按钮：
 *
 * @code
 *     new FilledButton {
 *         fbp::Child<DropdownMenu> {
 *             dmp::MenuWidget<DropdownMenuItem> { ... },
 *         },
 *     } + Row::Placement { 0 }
 * @endcode
 *
 * @note 该属性本质是转发构造，有 new 的行为
 */
template <class T>
    requires std::derived_from<T, QWidget>
struct Child {
    T* child_pointer = nullptr;

    explicit Child(T* pointer) noexcept
        : child_pointer { pointer } { }

    explicit Child(auto&&... args) noexcept
        requires std::constructible_from<T, decltype(args)...>
        : child_pointer { new T { std::forward<decltype(args)>(args)... } } { }

    friend auto dsl_invoke(QWidget& widget, const Child& prop) -> void {
        prop.child_pointer->setParent(&widget, prop.child_pointer->windowFlags());
    }
};

}
