#pragma once

#include <qlayout.h>

namespace creeper::api::pro {

/**
 * @brief 设置布局间距
 *
 * 调用 QLayout::setSpacing。
 */
struct Spacing {
    int value;

    constexpr explicit Spacing(int v) noexcept
        : value { v } { }

    friend auto dsl_invoke(auto& layout, const Spacing& prop) -> void {
        layout.setSpacing(prop.value);
    }
};

}
