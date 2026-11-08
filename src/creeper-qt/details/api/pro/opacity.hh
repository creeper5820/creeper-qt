#pragma once

namespace creeper::api::pro {

/// 设置组件透明度
/// 要求组件实现：void setOpacity(double)
struct Opacity {
    double value;

    constexpr explicit Opacity(double v) noexcept
        : value { v } { }

    friend auto dsl_invoke(auto& widget, const Opacity& prop) -> void {
        widget.setOpacity(prop.value);
    }
};

}
