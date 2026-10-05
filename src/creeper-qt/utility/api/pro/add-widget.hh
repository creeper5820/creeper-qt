#pragma once

#include "creeper-qt/utility/trait/widget.hh"

#include <concepts>
#include <utility>

namespace creeper::api::pro {

/**
 * @brief 把 widget 添加到布局
 *
 * @note 该属性本质是转发构造，有 new 的行为
 */
template <widget_trait T>
struct AddWidget {
    T* item_pointer = nullptr;

    explicit AddWidget(T* pointer) noexcept
        : item_pointer { pointer } { }

    explicit AddWidget(auto&&... args) noexcept
        requires std::constructible_from<T, decltype(args)...>
        : item_pointer { new T { std::forward<decltype(args)>(args)... } } { }

    friend auto dsl_invoke(auto& layout, const AddWidget& prop) -> void {
        layout.addWidget(prop.item_pointer);
    }
};

}
