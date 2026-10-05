#pragma once

#include <qpalette.h>
#include <qwidget.h>

namespace creeper::api::pro {

/**
 * @brief 设置背景角色
 *
 * 调用 QWidget::setBackgroundRole。
 */
struct BackgroundRole {
    QPalette::ColorRole value;

    explicit BackgroundRole(QPalette::ColorRole v) noexcept
        : value { v } { }

    friend auto dsl_invoke(QWidget& widget, const BackgroundRole& prop) -> void {
        widget.setBackgroundRole(prop.value);
    }
};

/**
 * @brief 设置前景角色
 *
 * 调用 QWidget::setForegroundRole。
 */
struct ForegroundRole {
    QPalette::ColorRole value;

    explicit ForegroundRole(QPalette::ColorRole v) noexcept
        : value { v } { }

    friend auto dsl_invoke(QWidget& widget, const ForegroundRole& prop) -> void {
        widget.setForegroundRole(prop.value);
    }
};

}
