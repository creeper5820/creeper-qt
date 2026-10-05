#pragma once

#include <qlayout.h>

namespace creeper::api::pro {

/**
 * @brief 设置布局对齐
 *
 * 调用 QLayout::setAlignment。
 */
struct Alignment {
    Qt::Alignment value;

    explicit Alignment(Qt::Alignment v) noexcept
        : value { v } { }

    friend auto dsl_invoke(auto& layout, const Alignment& prop) -> void {
        layout.setAlignment(prop.value);
    }
};

}
