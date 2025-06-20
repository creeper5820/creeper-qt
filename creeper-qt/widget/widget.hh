#pragma once

#include "creeper-qt/utility/wrapper/common-property.hh"
#include "creeper-qt/utility/wrapper/property.hh"

#include <qgraphicseffect.h>
#include <qwidget.h>

namespace creeper::widget {

namespace pro {
    using Token = common::Token<QWidget>;
    template <typename T>
    concept property_concept = std::derived_from<T, Token>;

    // 尺寸相关
    struct MinimumSize final : public QSize, Token {
        using QSize::QSize;
        void apply(QWidget& widget) const { widget.setMinimumSize(*this); }
    };
    struct MaximumSize final : public QSize, Token {
        using QSize::QSize;
        void apply(QWidget& widget) const { widget.setMaximumSize(*this); }
    };
    struct SizeIncrement final : public QSize, Token {
        using QSize::QSize;
        void apply(QWidget& widget) const { widget.setSizeIncrement(*this); }
    };
    struct BaseSize final : public QSize, Token {
        using QSize::QSize;
        void apply(QWidget& widget) const { widget.setBaseSize(*this); }
    };
    struct FixedSize final : public QSize, Token {
        using QSize::QSize;
        void apply(QWidget& widget) const { widget.setFixedSize(*this); }
    };

    struct MinimumWidth final : Token {
        int width;
        explicit MinimumWidth(int _) { width = _; }
        void apply(QWidget& widget) const { widget.setMinimumWidth(width); }
    };
    struct MaximumWidth final : Token {
        int width;
        explicit MaximumWidth(int _) { width = _; }
        void apply(QWidget& widget) const { widget.setMaximumWidth(width); }
    };
    struct FixedWidth final : Token {
        int width;
        explicit FixedWidth(int _) { width = _; }
        void apply(QWidget& widget) const { widget.setFixedWidth(width); }
    };

    struct MinimumHeight final : Token {
        int height;
        explicit MinimumHeight(int _) { height = _; }
        void apply(QWidget& widget) const { widget.setMinimumHeight(height); }
    };
    struct MaximumHeight final : Token {
        int height;
        explicit MaximumHeight(int _) { height = _; }
        void apply(QWidget& widget) const { widget.setMaximumHeight(height); }
    };
    struct FixedHeight final : Token {
        int height;
        explicit FixedHeight(int _) { height = _; }
        void apply(QWidget& widget) const { widget.setFixedHeight(height); }
    };

    // 显示相关
    struct Layout final : Token {
        QLayout* layout;
        explicit Layout(QLayout* _) { layout = _; }
        void apply(QWidget& widget) const { widget.setLayout(layout); }
    };
    struct LayoutDirection final : Token {
        Qt::LayoutDirection direction;
        explicit LayoutDirection(Qt::LayoutDirection _) { direction = _; }
        void apply(QWidget& widget) const { widget.setLayoutDirection(direction); }
    };
    struct BackgroundRole final : Token {
        QPalette::ColorRole role;
        explicit BackgroundRole(QPalette::ColorRole _) { role = _; }
        void apply(QWidget& widget) const { widget.setBackgroundRole(role); }
    };
    struct ForegroundRole final : Token {
        QPalette::ColorRole role;
        explicit ForegroundRole(QPalette::ColorRole _) { role = _; }
        void apply(QWidget& widget) const { widget.setForegroundRole(role); }
    };

    struct Font final : public QFont, Token {
        using QFont::QFont;
        void apply(QWidget& widget) const { widget.setFont(*this); }
    };

    struct BitmapMask final : public QBitmap, Token {
        using QBitmap::QBitmap;
        void apply(QWidget& widget) const { widget.setMask(*this); }
    };
    struct RegionMask final : public QRegion, Token {
        using QRegion::QRegion;
        void apply(QWidget& widget) const { widget.setMask(*this); }
    };
    struct ClearMask final : Token {
        void apply(QWidget& widget) const { widget.clearMask(); }
    };

    struct GraphicsEffect final : Token {
        QGraphicsEffect* effect;
        explicit GraphicsEffect(QGraphicsEffect* p) { effect = p; }
        void apply(QWidget& widget) const { widget.setGraphicsEffect(effect); }
    };

    // Window 相关
    struct WindowIcon final : public QIcon, Token {
        using QIcon::QIcon;
        void apply(QWidget& widget) const { widget.setWindowIcon(*this); }
    };
    struct WindowIconText final : public QString, Token {
        using QString::QString;
        void apply(QWidget& widget) const { widget.setWindowIconText(*this); }
    };
    struct WindowRole final : public QString, Token {
        using QString::QString;
        void apply(QWidget& widget) const { widget.setWindowRole(*this); }
    };
    struct WindowFilePath final : public QString, Token {
        using QString::QString;
        void apply(QWidget& widget) const { widget.setWindowFilePath(*this); }
    };
    struct WindowFlag final : Token {
        Qt::WindowType type;
        bool on;
        explicit WindowFlag(Qt::WindowType _, bool __ = true) { type = _, on = __; }
        void apply(QWidget& widget) const { widget.setWindowFlag(type, on); }
    };
    struct WindowFlags final : Token {
        Qt::WindowFlags flags;
        explicit WindowFlags(Qt::WindowFlags _) { flags = _; }
        void apply(QWidget& widget) const { widget.setWindowFlags(flags); }
    };
    struct WindowOpacity final : Token {
        double level;
        explicit WindowOpacity(double _) { level = _; }
        void apply(QWidget& widget) const { widget.setWindowOpacity(level); }
    };

    // 语言结构相关
    struct Parent final : Token {
        QWidget* parent;
        explicit Parent(QWidget* _) { parent = _; }
        void apply(QWidget& widget) const { widget.setParent(parent); }
    };

    /// @brief 绑定裸指针，为了避免赋值语句
    ///
    /// 一般情况下需要绑定指针：
    ///
    /// 假设下面是一个 layout 的内部
    /// clang-format 会在下面的赋值符号后换行
    /// @code
    ///     auto widget = (Widget*)nullptr;
    ///     ......
    ///     { widget =
    ///             new Widget {
    ///                 ......
    ///             } },
    ///     ......
    /// @endcode
    ///
    /// 利用赋值语句的返回特性将该组件返回给 layout，同时完成赋值，
    /// 但这样会多一层缩进，为保证构造配置的简洁和同一，可以使用该包装：
    ///
    /// @code
    ///     ......
    ///     { new Widget {
    ///         pro::Bind { widget },
    ///     } },
    ///     ......
    /// @endcode
    ///
    /// @tparam Final 需要绑定的组件类型（自动推导，无需显式指定）
    ///
    /// @date 2025-06-19
    template <class Final> struct Bind final : Token {
        Final*& widget;
        explicit Bind(Final*& widget) noexcept
            requires std::is_pointer<Final*>::value
            : widget(widget) { }
        void apply(Final& self) const noexcept { widget = &self; }
    };
}

}

namespace creeper {

class Widget : public QWidget {
    CREEPER_DEFINE_CONSTROCTOR(Widget, widget::pro);
    using QWidget::QWidget;
};

}
