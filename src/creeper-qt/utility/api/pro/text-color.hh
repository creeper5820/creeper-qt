#pragma once

#include <qcolor.h>
#include <utility>

namespace creeper::api::pro {

/// 通用文字颜色
/// 要求组件实现：void setTextColor(const QColor&)
struct TextColor {
    QColor value;

    explicit TextColor(QColor v) noexcept
        : value { std::move(v) } { }

    friend auto dsl_invoke(auto& widget, const TextColor& prop) -> void {
        widget.setTextColor(prop.value);
    }
};

}
