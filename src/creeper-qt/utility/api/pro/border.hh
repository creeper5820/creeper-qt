#pragma once

#include <qcolor.h>
#include <utility>

namespace creeper::api::pro {

/// 通用边界宽度
/// 要求组件实现：void setBorderWidth(double)
struct BorderWidth {
    double value;

    constexpr explicit BorderWidth(double v) noexcept
        : value { v } { }

    friend auto dsl_invoke(auto& widget, const BorderWidth& prop) -> void {
        widget.setBorderWidth(prop.value);
    }
};

/// 通用边界颜色
/// 要求组件实现：void setBorderColor(const QColor&)
struct BorderColor {
    QColor value;

    explicit BorderColor(QColor v) noexcept
        : value { std::move(v) } { }

    friend auto dsl_invoke(auto& widget, const BorderColor& prop) -> void {
        widget.setBorderColor(prop.value);
    }
};

}
