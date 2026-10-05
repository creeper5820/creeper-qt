#pragma once

#include <qfont.h>
#include <qstring.h>
#include <qwidget.h>

namespace creeper::api::pro {

/**
 * @brief 设置字体
 *
 * 调用 QWidget::setFont。
 */
struct Font : public QFont {
    using QFont::QFont;
    Font(const QFont& font)
        : QFont(font) { }

    friend auto dsl_invoke(QWidget& widget, const Font& prop) -> void {
        widget.setFont(static_cast<const QFont&>(prop));
    }
};

/**
 * @brief 设置提示文本
 *
 * 调用 QWidget::setToolTip。
 */
struct ToolTip : public QString {
    using QString::QString;
    ToolTip(const QString& tip)
        : QString(tip) { }

    friend auto dsl_invoke(QWidget& widget, const ToolTip& prop) -> void {
        widget.setToolTip(static_cast<const QString&>(prop));
    }
};

}
