#pragma once

#include <qbitmap.h>
#include <qicon.h>
#include <qwidget.h>

namespace cr::pro {

namespace internal {
    struct QWidgetProperty {
        virtual void apply(QWidget& widget) const = 0;
    };
}

// 尺寸相关
struct MinimumSize final : QSize, internal::QWidgetProperty {
    using QSize::QSize;
    void apply(QWidget& widget) const override { widget.setMinimumSize(*this); }
};
struct MaximumSize final : QSize, internal::QWidgetProperty {
    using QSize::QSize;
    void apply(QWidget& widget) const override { widget.setMaximumSize(*this); }
};
struct SizeIncrement final : QSize, internal::QWidgetProperty {
    using QSize::QSize;
    void apply(QWidget& widget) const override { widget.setSizeIncrement(*this); }
};
struct BaseSize final : QSize, internal::QWidgetProperty {
    using QSize::QSize;
    void apply(QWidget& widget) const override { widget.setBaseSize(*this); }
};
struct FixedSize final : QSize, internal::QWidgetProperty {
    using QSize::QSize;
    void apply(QWidget& widget) const override { widget.setFixedSize(*this); }
};

struct MinimumWidth final : internal::QWidgetProperty {
    int width;
    explicit MinimumWidth(int _) { width = _; }
    void apply(QWidget& widget) const override { widget.setMinimumWidth(width); }
};
struct MaximumWidth final : internal::QWidgetProperty {
    int width;
    explicit MaximumWidth(int _) { width = _; }
    void apply(QWidget& widget) const override { widget.setMaximumWidth(width); }
};
struct FixedWidth final : internal::QWidgetProperty {
    int width;
    explicit FixedWidth(int _) { width = _; }
    void apply(QWidget& widget) const override { widget.setFixedWidth(width); }
};

struct MinimumHeight final : internal::QWidgetProperty {
    int height;
    explicit MinimumHeight(int _) { height = _; }
    void apply(QWidget& widget) const override { widget.setMinimumHeight(height); }
};
struct MaximumHeight final : internal::QWidgetProperty {
    int height;
    explicit MaximumHeight(int _) { height = _; }
    void apply(QWidget& widget) const override { widget.setMaximumHeight(height); }
};
struct FixedHeight final : internal::QWidgetProperty {
    int height;
    explicit FixedHeight(int _) { height = _; }
    void apply(QWidget& widget) const override { widget.setFixedHeight(height); }
};

// 显示相关
struct BackgroundRole final : internal::QWidgetProperty {
    QPalette::ColorRole role;
    explicit BackgroundRole(QPalette::ColorRole _) { role = _; }
    void apply(QWidget& widget) const override { widget.setBackgroundRole(role); }
};
struct ForegroundRole final : internal::QWidgetProperty {
    QPalette::ColorRole role;
    explicit ForegroundRole(QPalette::ColorRole _) { role = _; }
    void apply(QWidget& widget) const override { widget.setForegroundRole(role); }
};

struct Font final : QFont, internal::QWidgetProperty {
    using QFont::QFont;
    void apply(QWidget& widget) const override { widget.setFont(*this); }
};

struct BitmapMask final : QBitmap, internal::QWidgetProperty {
    using QBitmap::QBitmap;
    void apply(QWidget& widget) const override { widget.setMask(*this); }
};
struct RegionMask final : QRegion, internal::QWidgetProperty {
    using QRegion::QRegion;
    void apply(QWidget& widget) const override { widget.setMask(*this); }
};
struct ClearMask final : internal::QWidgetProperty {
    void apply(QWidget& widget) const override { widget.clearMask(); }
};

// Window 相关
struct WindowIcon final : QIcon, internal::QWidgetProperty {
    using QIcon::QIcon;
    void apply(QWidget& widget) const override { widget.setWindowIcon(*this); }
};
struct WindowIconText final : QString, internal::QWidgetProperty {
    using QString::QString;
    void apply(QWidget& widget) const override { widget.setWindowIconText(*this); }
};
struct WindowRole final : QString, internal::QWidgetProperty {
    using QString::QString;
    void apply(QWidget& widget) const override { widget.setWindowRole(*this); }
};
struct WindowFilePath final : QString, internal::QWidgetProperty {
    using QString::QString;
    void apply(QWidget& widget) const override { widget.setWindowFilePath(*this); }
};
struct WindowFlag final : internal::QWidgetProperty {
    Qt::WindowType type;
    bool on;
    explicit WindowFlag(Qt::WindowType _, bool __ = true) { type = _, on = __; }
    void apply(QWidget& widget) const override { widget.setWindowFlag(type, on); }
};
struct WindowFlags final : internal::QWidgetProperty {
    Qt::WindowFlags flags;
    explicit WindowFlags(Qt::WindowFlags _) { flags = _; }
    void apply(QWidget& widget) const override { widget.setWindowFlags(flags); }
};
struct WindowOpacity final : internal::QWidgetProperty {
    double level;
    explicit WindowOpacity(double _) { level = _; }
    void apply(QWidget& widget) const override { widget.setWindowOpacity(level); }
};

template <typename Property>
concept qwidget_property_concept = std::is_base_of_v<internal::QWidgetProperty, Property>;

}
