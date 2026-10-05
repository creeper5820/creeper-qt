#pragma once

#include "creeper-qt/utility/qt_wrapper/margin-setter.hh"

namespace creeper::api::pro {

/**
 * @brief 设置布局四周边距
 */
struct Margin {
    int value;

    constexpr explicit Margin(int v) noexcept
        : value { v } { }

    friend auto dsl_invoke(auto& layout, const Margin& prop) -> void {
        qt::margin_setter(layout, prop.value);
    }
};

}
