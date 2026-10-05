#pragma once

#include <qgraphicseffect.h>
#include <qwidget.h>

namespace creeper::api::pro {

/**
 * @brief 设置图形效果
 *
 * 调用 QWidget::setGraphicsEffect。
 */
struct GraphicsEffect {
    QGraphicsEffect* value;

    explicit GraphicsEffect(QGraphicsEffect* v) noexcept
        : value { v } { }

    friend auto dsl_invoke(QWidget& widget, const GraphicsEffect& prop) -> void {
        widget.setGraphicsEffect(prop.value);
    }
};

}
