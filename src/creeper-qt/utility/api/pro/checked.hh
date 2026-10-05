#pragma once

namespace creeper::api::pro {

/// 通用 Checked 属性
/// 要求组件实现：void setChecked(bool)
struct Checked {
    bool value;

    constexpr explicit Checked(bool v) noexcept
        : value { v } { }

    friend auto dsl_invoke(auto& widget, const Checked& prop) -> void {
        widget.setChecked(prop.value);
    }
};

}
