#pragma once

#include <qicon.h>
#include <qscreen.h>
#include <qstring.h>
#include <qwidget.h>

namespace creeper::api::pro {

/**
 * @brief 设置布局方向
 *
 * 调用 QWidget::setLayoutDirection。
 */
struct LayoutDirection {
    Qt::LayoutDirection value;

    explicit LayoutDirection(Qt::LayoutDirection v) noexcept
        : value { v } { }

    friend auto dsl_invoke(QWidget& widget, const LayoutDirection& prop) -> void {
        widget.setLayoutDirection(prop.value);
    }
};

/**
 * @brief 设置单个窗口标志
 *
 * 调用 QWidget::setWindowFlag。
 */
struct WindowFlag {
    Qt::WindowType type;
    bool on;

    explicit WindowFlag(Qt::WindowType type, bool on = true) noexcept
        : type { type }
        , on { on } { }

    friend auto dsl_invoke(QWidget& widget, const WindowFlag& prop) -> void {
        widget.setWindowFlag(prop.type, prop.on);
    }
};

/**
 * @brief 设置窗口标志
 *
 * 调用 QWidget::setWindowFlags。
 */
struct WindowFlags {
    Qt::WindowFlags value;

    explicit WindowFlags(Qt::WindowFlags v) noexcept
        : value { v } { }

    friend auto dsl_invoke(QWidget& widget, const WindowFlags& prop) -> void {
        widget.setWindowFlags(prop.value);
    }
};

/**
 * @brief 设置窗口不透明度
 *
 * 调用 QWidget::setWindowOpacity。
 */
struct WindowOpacity {
    double value;

    constexpr explicit WindowOpacity(double v) noexcept
        : value { v } { }

    friend auto dsl_invoke(QWidget& widget, const WindowOpacity& prop) -> void {
        widget.setWindowOpacity(prop.value);
    }
};

/**
 * @brief 设置窗口图标
 *
 * 调用 QWidget::setWindowIcon。
 */
struct WindowIcon : public QIcon {
    using QIcon::QIcon;
    WindowIcon(const QIcon& icon)
        : QIcon(icon) { }

    friend auto dsl_invoke(QWidget& widget, const WindowIcon& prop) -> void {
        widget.setWindowIcon(static_cast<const QIcon&>(prop));
    }
};

/**
 * @brief 设置窗口图标文本
 *
 * 调用 QWidget::setWindowIconText。
 */
struct WindowIconText : public QString {
    using QString::QString;
    WindowIconText(const QString& text)
        : QString(text) { }

    friend auto dsl_invoke(QWidget& widget, const WindowIconText& prop) -> void {
        widget.setWindowIconText(static_cast<const QString&>(prop));
    }
};

/**
 * @brief 设置窗口角色
 *
 * 调用 QWidget::setWindowRole。
 */
struct WindowRole : public QString {
    using QString::QString;
    WindowRole(const QString& role)
        : QString(role) { }

    friend auto dsl_invoke(QWidget& widget, const WindowRole& prop) -> void {
        widget.setWindowRole(static_cast<const QString&>(prop));
    }
};

/**
 * @brief 设置窗口文件路径
 *
 * 调用 QWidget::setWindowFilePath。
 */
struct WindowFilePath : public QString {
    using QString::QString;
    WindowFilePath(const QString& path)
        : QString(path) { }

    friend auto dsl_invoke(QWidget& widget, const WindowFilePath& prop) -> void {
        widget.setWindowFilePath(static_cast<const QString&>(prop));
    }
};

/**
 * @brief 将窗口移动到屏幕中心
 */
struct MoveCenter {
    friend auto dsl_invoke(QWidget& widget, const MoveCenter&) -> void {
        const auto screen          = widget.screen();
        const auto screen_geometry = screen->availableGeometry();
        const auto screen_width    = screen_geometry.width();
        const auto screen_height   = screen_geometry.height();

        const auto widget_geometry = widget.geometry();
        const auto widget_width    = widget_geometry.width();
        const auto widget_height   = widget_geometry.height();

        const auto x = (screen_width - widget_width) / 2;
        const auto y = (screen_height - widget_height) / 2;

        widget.move(x, y);
    }
};

}
