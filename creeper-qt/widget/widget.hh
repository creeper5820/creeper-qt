#pragma once

#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/property.hh"

#include <qgraphicseffect.h>
#include <qwidget.h>

namespace creeper::widget {

namespace pro {
    using Token = common::Token<QWidget>;

    // 尺寸相关
    struct SizePolicy : Token {
        QSizePolicy::Policy v, h;
        explicit SizePolicy(QSizePolicy::Policy policy)
            : v { policy }
            , h { policy } { }
        explicit SizePolicy(QSizePolicy::Policy v, QSizePolicy::Policy h)
            : v { v }
            , h { h } { }
        void apply(QWidget& self) const { self.setSizePolicy(h, v); }
    };
    struct MinimumSize : public QSize, Token {
        using QSize::QSize;
        explicit MinimumSize(const QSize& size)
            : QSize { size } { }
        void apply(QWidget& widget) const { widget.setMinimumSize(*this); }
    };
    struct MaximumSize : public QSize, Token {
        using QSize::QSize;
        explicit MaximumSize(const QSize& size)
            : QSize { size } { }
        void apply(QWidget& widget) const { widget.setMaximumSize(*this); }
    };
    struct SizeIncrement : public QSize, Token {
        using QSize::QSize;
        explicit SizeIncrement(const QSize& size)
            : QSize { size } { }
        void apply(QWidget& widget) const { widget.setSizeIncrement(*this); }
    };
    struct BaseSize : public QSize, Token {
        using QSize::QSize;
        explicit BaseSize(const QSize& size)
            : QSize { size } { }
        void apply(QWidget& widget) const { widget.setBaseSize(*this); }
    };
    struct FixedSize : public QSize, Token {
        using QSize::QSize;
        explicit FixedSize(const QSize& size)
            : QSize { size } { }
        void apply(QWidget& widget) const { widget.setFixedSize(*this); }
    };

    struct MinimumWidth : Token {
        int width;
        explicit MinimumWidth(int _) { width = _; }
        void apply(QWidget& widget) const { widget.setMinimumWidth(width); }
    };
    struct MaximumWidth : Token {
        int width;
        explicit MaximumWidth(int _) { width = _; }
        void apply(QWidget& widget) const { widget.setMaximumWidth(width); }
    };
    struct FixedWidth : Token {
        int width;
        explicit FixedWidth(int _) { width = _; }
        void apply(QWidget& widget) const { widget.setFixedWidth(width); }
    };

    struct MinimumHeight : Token {
        int height;
        explicit MinimumHeight(int _) { height = _; }
        void apply(QWidget& widget) const { widget.setMinimumHeight(height); }
    };
    struct MaximumHeight : Token {
        int height;
        explicit MaximumHeight(int _) { height = _; }
        void apply(QWidget& widget) const { widget.setMaximumHeight(height); }
    };
    struct FixedHeight : Token {
        int height;
        explicit FixedHeight(int _) { height = _; }
        void apply(QWidget& widget) const { widget.setFixedHeight(height); }
    };

    // 显示相关

    /// @note 该属性本质是转发构造，有 new 的行为
    template <class T> struct Layout : Token {
        T* layout_;

        explicit Layout(T* pointer) noexcept
            requires std::convertible_to<decltype(pointer), QLayout*>
            : layout_ { pointer } { }

        explicit Layout(auto&&... args)
            requires std::constructible_from<T, decltype(args)...>
            : layout_ { new T { std::forward<decltype(args)>(args)... } } { }

        void apply(QWidget& widget) const { widget.setLayout(layout_); }
    };
    struct LayoutDirection : Token {
        Qt::LayoutDirection direction;
        explicit LayoutDirection(Qt::LayoutDirection _) { direction = _; }
        void apply(QWidget& widget) const { widget.setLayoutDirection(direction); }
    };
    struct BackgroundRole : Token {
        QPalette::ColorRole role;
        explicit BackgroundRole(QPalette::ColorRole _) { role = _; }
        void apply(QWidget& widget) const { widget.setBackgroundRole(role); }
    };
    struct ForegroundRole : Token {
        QPalette::ColorRole role;
        explicit ForegroundRole(QPalette::ColorRole _) { role = _; }
        void apply(QWidget& widget) const { widget.setForegroundRole(role); }
    };

    struct Font : public QFont, Token {
        using QFont::QFont;
        explicit Font(const QFont& font)
            : QFont { font } { }
        void apply(QWidget& widget) const { widget.setFont(*this); }
    };

    struct BitmapMask : public QBitmap, Token {
        using QBitmap::QBitmap;
        void apply(QWidget& widget) const { widget.setMask(*this); }
    };
    struct RegionMask : public QRegion, Token {
        using QRegion::QRegion;
        void apply(QWidget& widget) const { widget.setMask(*this); }
    };
    struct ClearMask : Token {
        void apply(QWidget& widget) const { widget.clearMask(); }
    };

    struct GraphicsEffect : Token {
        QGraphicsEffect* effect;
        explicit GraphicsEffect(QGraphicsEffect* p) { effect = p; }
        void apply(QWidget& widget) const { widget.setGraphicsEffect(effect); }
    };

    // Window 相关
    struct WindowIcon : public QIcon, Token {
        using QIcon::QIcon;
        void apply(QWidget& widget) const { widget.setWindowIcon(*this); }
    };
    struct WindowIconText : public QString, Token {
        using QString::QString;
        void apply(QWidget& widget) const { widget.setWindowIconText(*this); }
    };
    struct WindowRole : public QString, Token {
        using QString::QString;
        void apply(QWidget& widget) const { widget.setWindowRole(*this); }
    };
    struct WindowFilePath : public QString, Token {
        using QString::QString;
        void apply(QWidget& widget) const { widget.setWindowFilePath(*this); }
    };
    struct WindowFlag : Token {
        Qt::WindowType type;
        bool on;
        explicit WindowFlag(Qt::WindowType _, bool __ = true) { type = _, on = __; }
        void apply(QWidget& widget) const { widget.setWindowFlag(type, on); }
    };
    struct WindowFlags : Token {
        Qt::WindowFlags flags;
        explicit WindowFlags(Qt::WindowFlags _) { flags = _; }
        void apply(QWidget& widget) const { widget.setWindowFlags(flags); }
    };
    struct WindowOpacity : Token {
        double level;
        explicit WindowOpacity(double _) { level = _; }
        void apply(QWidget& widget) const { widget.setWindowOpacity(level); }
    };

    // 语言结构相关
    struct Parent : Token {
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
    template <class Final> struct Bind : Token {
        Final*& widget;
        explicit Bind(Final*& widget) noexcept
            requires std::is_pointer<Final*>::value
            : widget(widget) { }
        void apply(Final& self) const noexcept { widget = &self; }
    };

    // 传入一个方法用来辅助构造，在没有想要的接口时用这个吧
    template <typename Lambda> struct Apply : Token {
        Lambda lambda;
        explicit Apply(Lambda lambda) noexcept
            : lambda { lambda } { }
        auto apply(auto& self) const noexcept -> void {
            if constexpr (std::invocable<Lambda>) lambda();
            if constexpr (std::invocable<Lambda, decltype(self)>) lambda(self);
        }
    };

    template <typename T>
    concept trait = std::derived_from<T, Token>;

    CREEPER_DEFINE_CHECK(trait);
}

}

namespace creeper {
using Widget = Declarative<QWidget, widget::pro::checker>;
}
