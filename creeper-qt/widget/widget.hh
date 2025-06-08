#pragma once

#include "utility/wrapper/property.hh"
#include <qwidget.h>

namespace creeper {

namespace pro::widget {
    using Property = InternalProperty<QWidget>;
    template <typename T>
    concept property_concept = std::derived_from<T, Property>;

    // 尺寸相关
    struct MinimumSize final : public QSize, Property {
        using QSize::QSize;
        void apply(QWidget& widget) const override { widget.setMinimumSize(*this); }
    };
    struct MaximumSize final : public QSize, Property {
        using QSize::QSize;
        void apply(QWidget& widget) const override { widget.setMaximumSize(*this); }
    };
    struct SizeIncrement final : public QSize, Property {
        using QSize::QSize;
        void apply(QWidget& widget) const override { widget.setSizeIncrement(*this); }
    };
    struct BaseSize final : public QSize, Property {
        using QSize::QSize;
        void apply(QWidget& widget) const override { widget.setBaseSize(*this); }
    };
    struct FixedSize final : public QSize, Property {
        using QSize::QSize;
        void apply(QWidget& widget) const override { widget.setFixedSize(*this); }
    };

    struct MinimumWidth final : Property {
        int width;
        explicit MinimumWidth(int _) { width = _; }
        void apply(QWidget& widget) const override { widget.setMinimumWidth(width); }
    };
    struct MaximumWidth final : Property {
        int width;
        explicit MaximumWidth(int _) { width = _; }
        void apply(QWidget& widget) const override { widget.setMaximumWidth(width); }
    };
    struct FixedWidth final : Property {
        int width;
        explicit FixedWidth(int _) { width = _; }
        void apply(QWidget& widget) const override { widget.setFixedWidth(width); }
    };

    struct MinimumHeight final : Property {
        int height;
        explicit MinimumHeight(int _) { height = _; }
        void apply(QWidget& widget) const override { widget.setMinimumHeight(height); }
    };
    struct MaximumHeight final : Property {
        int height;
        explicit MaximumHeight(int _) { height = _; }
        void apply(QWidget& widget) const override { widget.setMaximumHeight(height); }
    };
    struct FixedHeight final : Property {
        int height;
        explicit FixedHeight(int _) { height = _; }
        void apply(QWidget& widget) const override { widget.setFixedHeight(height); }
    };

    // 显示相关
    struct Layout final : Property {
        QLayout* layout;
        explicit Layout(QLayout* _) { layout = _; }
        void apply(QWidget& widget) const override { widget.setLayout(layout); }
    };
    struct LayoutDirection final : Property {
        Qt::LayoutDirection direction;
        explicit LayoutDirection(Qt::LayoutDirection _) { direction = _; }
        void apply(QWidget& widget) const override { widget.setLayoutDirection(direction); }
    };
    struct BackgroundRole final : Property {
        QPalette::ColorRole role;
        explicit BackgroundRole(QPalette::ColorRole _) { role = _; }
        void apply(QWidget& widget) const override { widget.setBackgroundRole(role); }
    };
    struct ForegroundRole final : Property {
        QPalette::ColorRole role;
        explicit ForegroundRole(QPalette::ColorRole _) { role = _; }
        void apply(QWidget& widget) const override { widget.setForegroundRole(role); }
    };

    struct Font final : public QFont, Property {
        using QFont::QFont;
        void apply(QWidget& widget) const override { widget.setFont(*this); }
    };

    struct BitmapMask final : public QBitmap, Property {
        using QBitmap::QBitmap;
        void apply(QWidget& widget) const override { widget.setMask(*this); }
    };
    struct RegionMask final : public QRegion, Property {
        using QRegion::QRegion;
        void apply(QWidget& widget) const override { widget.setMask(*this); }
    };
    struct ClearMask final : Property {
        void apply(QWidget& widget) const override { widget.clearMask(); }
    };

    // Window 相关
    struct WindowIcon final : public QIcon, Property {
        using QIcon::QIcon;
        void apply(QWidget& widget) const override { widget.setWindowIcon(*this); }
    };
    struct WindowIconText final : public QString, Property {
        using QString::QString;
        void apply(QWidget& widget) const override { widget.setWindowIconText(*this); }
    };
    struct WindowRole final : public QString, Property {
        using QString::QString;
        void apply(QWidget& widget) const override { widget.setWindowRole(*this); }
    };
    struct WindowFilePath final : public QString, Property {
        using QString::QString;
        void apply(QWidget& widget) const override { widget.setWindowFilePath(*this); }
    };
    struct WindowFlag final : Property {
        Qt::WindowType type;
        bool on;
        explicit WindowFlag(Qt::WindowType _, bool __ = true) { type = _, on = __; }
        void apply(QWidget& widget) const override { widget.setWindowFlag(type, on); }
    };
    struct WindowFlags final : Property {
        Qt::WindowFlags flags;
        explicit WindowFlags(Qt::WindowFlags _) { flags = _; }
        void apply(QWidget& widget) const override { widget.setWindowFlags(flags); }
    };
    struct WindowOpacity final : Property {
        double level;
        explicit WindowOpacity(double _) { level = _; }
        void apply(QWidget& widget) const override { widget.setWindowOpacity(level); }
    };
}

class Widget : public QWidget {
    CREEPER_DEFINE_CONSTROCTOR(Widget, pro::widget);
    using QWidget::QWidget;
};

}
