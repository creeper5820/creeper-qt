#pragma once

#include <qcolor.h>

#include <utility>

namespace creeper::api::pro {

/// 通用水波纹颜色
/// 要求组件实现：void setWaterColor(const QColor&)
struct WaterColor {
    QColor value;

    explicit WaterColor(QColor v) noexcept
        : value { std::move(v) } { }

    friend auto dsl_invoke(auto& widget, const WaterColor& prop) -> void {
        widget.setWaterColor(prop.value);
    }
};

}
