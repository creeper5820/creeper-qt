#pragma once
#include <concepts>
#include <qwidget.h>

namespace creeper {

template <class T>
concept widget_trait = std::convertible_to<T*, QWidget*>;

template <class T>
concept widget_pointer_trait = std::convertible_to<T, QWidget*>;

template <class T>
concept layout_trait = std::convertible_to<T*, QLayout*>;

template <class T>
concept layout_pointer_trait = std::convertible_to<T, QLayout*>;

template <class T>
concept item_trait = widget_trait<T> || layout_trait<T>;

template <class T>
concept container_trait = requires(T t) {
    { t.addWidget(std::declval<QWidget*>(), int {}, Qt::AlignCenter) };
    { t.addLayout(std::declval<QLayout*>(), int {}) };
};

template <class T>
concept area_trait = requires(T t) {
    { t.setWidget(std::declval<QWidget*>()) };
    { t.setLayout(std::declval<QLayout*>()) };
};

template <class T>
concept selectable_trait = requires(T t) {
    { std::as_const(t).selected() } -> std::convertible_to<bool>;
    { t.set_selected(bool {}) };
};

}
