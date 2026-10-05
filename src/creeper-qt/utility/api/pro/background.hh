#pragma once

#include <qcolor.h>
#include <utility>

namespace creeper::api::pro {

/// 通用背景颜色
/// 要求组件实现：void setBackground(const QColor&)
struct Background {
    QColor value;

    explicit Background(QColor v) noexcept
        : value { std::move(v) } { }

    friend auto dsl_invoke(auto& widget, const Background& prop) -> void {
        widget.setBackground(prop.value);
    }
};

}
