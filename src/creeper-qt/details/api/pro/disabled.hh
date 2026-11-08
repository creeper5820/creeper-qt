#pragma once

namespace creeper::api::pro {

/// 通用禁止属性
/// 要求组件实现：void setDisabled(bool)
struct Disabled {
    bool value;

    constexpr explicit Disabled(bool v) noexcept
        : value { v } { }

    friend auto dsl_invoke(auto& widget, const Disabled& prop) -> void {
        widget.setDisabled(prop.value);
    }
};

}
