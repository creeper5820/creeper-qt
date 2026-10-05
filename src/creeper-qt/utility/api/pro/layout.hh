#pragma once

#include <concepts>
#include <qlayout.h>
#include <qwidget.h>
#include <utility>

namespace creeper::api::pro {

/**
 * @brief 为组件设置布局
 *
 * @note 该属性本质是转发构造，有 new 的行为
 */
template <class T>
    requires std::convertible_to<T*, QLayout*>
struct Layout {
    T* layout_;

    explicit Layout(T* pointer) noexcept
        : layout_ { pointer } { }

    explicit Layout(auto&&... args)
        requires std::constructible_from<T, decltype(args)...>
        : layout_ { new T { std::forward<decltype(args)>(args)... } } { }

    explicit Layout(auto&&... args) noexcept {
        // 实例化一次错误的构造，让错误爆出来
        T { std::forward<decltype(args)>(args)... };
    }

    friend auto dsl_invoke(QWidget& widget, const Layout& prop) -> void {
        widget.setLayout(prop.layout_);
    }
};

}

namespace creeper {

/**
 * @brief 布局指针的隐式转发
 *
 * 使 `Widget { new Row { ... } }` 等价于 `Layout<Row> { ... }`。
 */
template <class T>
    requires std::derived_from<T, QLayout>
auto dsl_invoke(QWidget& widget, T* layout) -> void {
    widget.setLayout(layout);
}

}
