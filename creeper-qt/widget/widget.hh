#pragma once

#include "utility/wrapper/property.hh"
#include <qgraphicseffect.h>
#include <qwidget.h>

namespace creeper::widget {

namespace pro {
    using Property = common::Token<QWidget>;
    template <typename T>
    concept property_concept = std::derived_from<T, Property>;

    // 尺寸相关
    struct MinimumSize final : public QSize, Property {
        using QSize::QSize;
        void apply(QWidget& widget) const { widget.setMinimumSize(*this); }
    };
    struct MaximumSize final : public QSize, Property {
        using QSize::QSize;
        void apply(QWidget& widget) const { widget.setMaximumSize(*this); }
    };
    struct SizeIncrement final : public QSize, Property {
        using QSize::QSize;
        void apply(QWidget& widget) const { widget.setSizeIncrement(*this); }
    };
    struct BaseSize final : public QSize, Property {
        using QSize::QSize;
        void apply(QWidget& widget) const { widget.setBaseSize(*this); }
    };
    struct FixedSize final : public QSize, Property {
        using QSize::QSize;
        void apply(QWidget& widget) const { widget.setFixedSize(*this); }
    };

    struct MinimumWidth final : Property {
        int width;
        explicit MinimumWidth(int _) { width = _; }
        void apply(QWidget& widget) const { widget.setMinimumWidth(width); }
    };
    struct MaximumWidth final : Property {
        int width;
        explicit MaximumWidth(int _) { width = _; }
        void apply(QWidget& widget) const { widget.setMaximumWidth(width); }
    };
    struct FixedWidth final : Property {
        int width;
        explicit FixedWidth(int _) { width = _; }
        void apply(QWidget& widget) const { widget.setFixedWidth(width); }
    };

    struct MinimumHeight final : Property {
        int height;
        explicit MinimumHeight(int _) { height = _; }
        void apply(QWidget& widget) const { widget.setMinimumHeight(height); }
    };
    struct MaximumHeight final : Property {
        int height;
        explicit MaximumHeight(int _) { height = _; }
        void apply(QWidget& widget) const { widget.setMaximumHeight(height); }
    };
    struct FixedHeight final : Property {
        int height;
        explicit FixedHeight(int _) { height = _; }
        void apply(QWidget& widget) const { widget.setFixedHeight(height); }
    };

    // 显示相关
    struct Layout final : Property {
        QLayout* layout;
        explicit Layout(QLayout* _) { layout = _; }
        void apply(QWidget& widget) const { widget.setLayout(layout); }
    };
    struct LayoutDirection final : Property {
        Qt::LayoutDirection direction;
        explicit LayoutDirection(Qt::LayoutDirection _) { direction = _; }
        void apply(QWidget& widget) const { widget.setLayoutDirection(direction); }
    };
    struct BackgroundRole final : Property {
        QPalette::ColorRole role;
        explicit BackgroundRole(QPalette::ColorRole _) { role = _; }
        void apply(QWidget& widget) const { widget.setBackgroundRole(role); }
    };
    struct ForegroundRole final : Property {
        QPalette::ColorRole role;
        explicit ForegroundRole(QPalette::ColorRole _) { role = _; }
        void apply(QWidget& widget) const { widget.setForegroundRole(role); }
    };

    struct Font final : public QFont, Property {
        using QFont::QFont;
        void apply(QWidget& widget) const { widget.setFont(*this); }
    };

    struct BitmapMask final : public QBitmap, Property {
        using QBitmap::QBitmap;
        void apply(QWidget& widget) const { widget.setMask(*this); }
    };
    struct RegionMask final : public QRegion, Property {
        using QRegion::QRegion;
        void apply(QWidget& widget) const { widget.setMask(*this); }
    };
    struct ClearMask final : Property {
        void apply(QWidget& widget) const { widget.clearMask(); }
    };

    struct GraphicsEffect final : Property {
        QGraphicsEffect* effect;
        explicit GraphicsEffect(QGraphicsEffect* p) { effect = p; }
        void apply(QWidget& widget) const { widget.setGraphicsEffect(effect); }
    };

    // Window 相关
    struct WindowIcon final : public QIcon, Property {
        using QIcon::QIcon;
        void apply(QWidget& widget) const { widget.setWindowIcon(*this); }
    };
    struct WindowIconText final : public QString, Property {
        using QString::QString;
        void apply(QWidget& widget) const { widget.setWindowIconText(*this); }
    };
    struct WindowRole final : public QString, Property {
        using QString::QString;
        void apply(QWidget& widget) const { widget.setWindowRole(*this); }
    };
    struct WindowFilePath final : public QString, Property {
        using QString::QString;
        void apply(QWidget& widget) const { widget.setWindowFilePath(*this); }
    };
    struct WindowFlag final : Property {
        Qt::WindowType type;
        bool on;
        explicit WindowFlag(Qt::WindowType _, bool __ = true) { type = _, on = __; }
        void apply(QWidget& widget) const { widget.setWindowFlag(type, on); }
    };
    struct WindowFlags final : Property {
        Qt::WindowFlags flags;
        explicit WindowFlags(Qt::WindowFlags _) { flags = _; }
        void apply(QWidget& widget) const { widget.setWindowFlags(flags); }
    };
    struct WindowOpacity final : Property {
        double level;
        explicit WindowOpacity(double _) { level = _; }
        void apply(QWidget& widget) const { widget.setWindowOpacity(level); }
    };

    // 语言结构相关
    struct Parent final : Property {
        QWidget* parent;
        explicit Parent(QWidget* _) { parent = _; }
        void apply(QWidget& widget) const { widget.setParent(parent); }
    };
}

}

namespace creeper {

class Widget : public QWidget {
    CREEPER_DEFINE_CONSTROCTOR(Widget, widget::pro);
    using QWidget::QWidget;
};

}
