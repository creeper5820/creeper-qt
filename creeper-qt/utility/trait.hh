#pragma once
#include <concepts>
#include <qwidget.h>

namespace creeper {

template <class T>
concept widget_trait = std::convertible_to<T*, QWidget*>;

template <class T>
concept layout_trait = std::convertible_to<T*, QLayout*>;

template <class T>
concept container_trait = requires(T t) {
    { t.addWidget(nullptr, int {}, Qt::AlignCenter) };
    { t.addLayout(nullptr, int {}) };
};

}
