#pragma once

#include <qsize.h>
#include <qwidget.h>

namespace creeper::api::pro {

/**
 * @brief 设置最小宽度
 *
 * 调用 QWidget::setMinimumWidth。
 */
struct MinimumWidth {
    int value;

    constexpr explicit MinimumWidth(int v) noexcept
        : value { v } { }

    friend auto dsl_invoke(QWidget& widget, const MinimumWidth& prop) -> void {
        widget.setMinimumWidth(prop.value);
    }
};

/**
 * @brief 设置最大宽度
 *
 * 调用 QWidget::setMaximumWidth。
 */
struct MaximumWidth {
    int value;

    constexpr explicit MaximumWidth(int v) noexcept
        : value { v } { }

    friend auto dsl_invoke(QWidget& widget, const MaximumWidth& prop) -> void {
        widget.setMaximumWidth(prop.value);
    }
};

/**
 * @brief 设置固定宽度
 *
 * 调用 QWidget::setFixedWidth。
 */
struct FixedWidth {
    int value;

    constexpr explicit FixedWidth(int v) noexcept
        : value { v } { }

    friend auto dsl_invoke(QWidget& widget, const FixedWidth& prop) -> void {
        widget.setFixedWidth(prop.value);
    }
};

/**
 * @brief 设置最小高度
 *
 * 调用 QWidget::setMinimumHeight。
 */
struct MinimumHeight {
    int value;

    constexpr explicit MinimumHeight(int v) noexcept
        : value { v } { }

    friend auto dsl_invoke(QWidget& widget, const MinimumHeight& prop) -> void {
        widget.setMinimumHeight(prop.value);
    }
};

/**
 * @brief 设置最大高度
 *
 * 调用 QWidget::setMaximumHeight。
 */
struct MaximumHeight {
    int value;

    constexpr explicit MaximumHeight(int v) noexcept
        : value { v } { }

    friend auto dsl_invoke(QWidget& widget, const MaximumHeight& prop) -> void {
        widget.setMaximumHeight(prop.value);
    }
};

/**
 * @brief 设置固定高度
 *
 * 调用 QWidget::setFixedHeight。
 */
struct FixedHeight {
    int value;

    constexpr explicit FixedHeight(int v) noexcept
        : value { v } { }

    friend auto dsl_invoke(QWidget& widget, const FixedHeight& prop) -> void {
        widget.setFixedHeight(prop.value);
    }
};

/**
 * @brief 设置最小尺寸
 *
 * 调用 QWidget::setMinimumSize。
 */
struct MinimumSize : public QSize {
    using QSize::QSize;
    MinimumSize(const QSize& size)
        : QSize(size) { }

    friend auto dsl_invoke(QWidget& widget, const MinimumSize& prop) -> void {
        widget.setMinimumSize(static_cast<const QSize&>(prop));
    }
};

/**
 * @brief 设置最大尺寸
 *
 * 调用 QWidget::setMaximumSize。
 */
struct MaximumSize : public QSize {
    using QSize::QSize;
    MaximumSize(const QSize& size)
        : QSize(size) { }

    friend auto dsl_invoke(QWidget& widget, const MaximumSize& prop) -> void {
        widget.setMaximumSize(static_cast<const QSize&>(prop));
    }
};

/**
 * @brief 设置尺寸增量
 *
 * 调用 QWidget::setSizeIncrement。
 */
struct SizeIncrement : public QSize {
    using QSize::QSize;
    SizeIncrement(const QSize& size)
        : QSize(size) { }

    friend auto dsl_invoke(QWidget& widget, const SizeIncrement& prop) -> void {
        widget.setSizeIncrement(static_cast<const QSize&>(prop));
    }
};

/**
 * @brief 设置基础尺寸
 *
 * 调用 QWidget::setBaseSize。
 */
struct BaseSize : public QSize {
    using QSize::QSize;
    BaseSize(const QSize& size)
        : QSize(size) { }

    friend auto dsl_invoke(QWidget& widget, const BaseSize& prop) -> void {
        widget.setBaseSize(static_cast<const QSize&>(prop));
    }
};

/**
 * @brief 设置固定尺寸
 *
 * 调用 QWidget::setFixedSize。
 */
struct FixedSize : public QSize {
    using QSize::QSize;
    FixedSize(const QSize& size)
        : QSize(size) { }

    friend auto dsl_invoke(QWidget& widget, const FixedSize& prop) -> void {
        widget.setFixedSize(static_cast<const QSize&>(prop));
    }
};

/**
 * @brief 设置尺寸策略
 *
 * 调用 QWidget::setSizePolicy。
 */
struct SizePolicy {
    QSizePolicy::Policy v, h;

    explicit SizePolicy(QSizePolicy::Policy policy) noexcept
        : v { policy }
        , h { policy } { }

    explicit SizePolicy(QSizePolicy::Policy v, QSizePolicy::Policy h) noexcept
        : v { v }
        , h { h } { }

    friend auto dsl_invoke(QWidget& widget, const SizePolicy& prop) -> void {
        widget.setSizePolicy(prop.h, prop.v);
    }
};

}
