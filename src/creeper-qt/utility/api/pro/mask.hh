#pragma once

#include <qbitmap.h>
#include <qregion.h>
#include <qwidget.h>

namespace creeper::api::pro {

/**
 * @brief 清除遮罩
 *
 * 调用 QWidget::clearMask。
 */
struct ClearMask {
    explicit ClearMask() noexcept = default;

    friend auto dsl_invoke(QWidget& widget, const ClearMask&) -> void { widget.clearMask(); }
};

/**
 * @brief 设置位图遮罩
 *
 * 调用 QWidget::setMask。
 */
struct BitmapMask : public QBitmap {
    using QBitmap::QBitmap;
    BitmapMask(const QBitmap& bitmap)
        : QBitmap(bitmap) { }

    friend auto dsl_invoke(QWidget& widget, const BitmapMask& prop) -> void {
        widget.setMask(static_cast<const QBitmap&>(prop));
    }
};

/**
 * @brief 设置区域遮罩
 *
 * 调用 QWidget::setMask。
 */
struct RegionMask : public QRegion {
    using QRegion::QRegion;
    RegionMask(const QRegion& region)
        : QRegion(region) { }

    friend auto dsl_invoke(QWidget& widget, const RegionMask& prop) -> void {
        widget.setMask(static_cast<const QRegion&>(prop));
    }
};

}
