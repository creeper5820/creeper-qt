#pragma once

#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/property.hh"

#include <qgraphicseffect.h>
#include <qscreen.h>

namespace creeper::widget::pro {

using Token = common::Token<QWidget>;

using MinimumWidth  = SetterProp<Token, int, [](auto& self, int v) { self.setMinimumWidth(v); }>;
using MaximumWidth  = SetterProp<Token, int, [](auto& self, int v) { self.setMaximumWidth(v); }>;
using FixedWidth    = SetterProp<Token, int, [](auto& self, int v) { self.setFixedWidth(v); }>;
using MinimumHeight = SetterProp<Token, int, [](auto& self, int v) { self.setMinimumHeight(v); }>;
using MaximumHeight = SetterProp<Token, int, [](auto& self, int v) { self.setMaximumHeight(v); }>;
using FixedHeight   = SetterProp<Token, int, [](auto& self, int v) { self.setFixedHeight(v); }>;

using LayoutDirection = SetterProp<Token, Qt::LayoutDirection,
    [](auto& self, Qt::LayoutDirection v) { self.setLayoutDirection(v); }>;
using BackgroundRole  = SetterProp<Token, QPalette::ColorRole,
     [](auto& self, QPalette::ColorRole v) { self.setBackgroundRole(v); }>;
using ForegroundRole  = SetterProp<Token, QPalette::ColorRole,
     [](auto& self, QPalette::ColorRole v) { self.setForegroundRole(v); }>;

using ClearMask = SetterProp<Token, std::nullptr_t, [](auto& self, auto) { self.clearMask(); }>;
using GraphicsEffect = SetterProp<Token, QGraphicsEffect*,
    [](auto& self, QGraphicsEffect* v) { self.setGraphicsEffect(v); }>;

struct WindowFlag : Token {
    Qt::WindowType type;
    bool on;

    explicit WindowFlag(Qt::WindowType type, bool on = true)
        : type { type }
        , on { on } { }

    void apply(QWidget& widget) const { widget.setWindowFlag(type, on); }
};
using WindowFlags = SetterProp<Token, Qt::WindowFlags,
    [](auto& self, Qt::WindowFlags v) { self.setWindowFlags(v); }>;
using WindowOpacity =
    SetterProp<Token, double, [](auto& self, double v) { self.setWindowOpacity(v); }>;

using Parent = SetterProp<Token, QWidget*, [](auto& self, QWidget* v) { self.setParent(v); }>;
using Child  = SetterProp<Token, QWidget*, [](auto& self, QWidget* v) { v->setParent(&self); }>;

using MinimumSize =
    DerivedProp<Token, QSize, [](auto& self, const QSize& v) { self.setMinimumSize(v); }>;
using MaximumSize =
    DerivedProp<Token, QSize, [](auto& self, const QSize& v) { self.setMaximumSize(v); }>;
using SizeIncrement =
    DerivedProp<Token, QSize, [](auto& self, const QSize& v) { self.setSizeIncrement(v); }>;
using BaseSize = DerivedProp<Token, QSize, [](auto& self, const QSize& v) { self.setBaseSize(v); }>;
using FixedSize =
    DerivedProp<Token, QSize, [](auto& self, const QSize& v) { self.setFixedSize(v); }>;

using Font = DerivedProp<Token, QFont, [](auto& self, const QFont& v) { self.setFont(v); }>;
using BitmapMask =
    DerivedProp<Token, QBitmap, [](auto& self, const QBitmap& v) { self.setMask(v); }>;
using RegionMask =
    DerivedProp<Token, QRegion, [](auto& self, const QRegion& v) { self.setMask(v); }>;
using WindowIcon =
    DerivedProp<Token, QIcon, [](auto& self, const QIcon& v) { self.setWindowIcon(v); }>;
using WindowIconText =
    DerivedProp<Token, QString, [](auto& self, const QString& v) { self.setWindowIconText(v); }>;
using WindowRole =
    DerivedProp<Token, QString, [](auto& self, const QString& v) { self.setWindowRole(v); }>;
using WindowFilePath =
    DerivedProp<Token, QString, [](auto& self, const QString& v) { self.setWindowFilePath(v); }>;

using ToolTip =
    DerivedProp<Token, QString, [](auto& self, const QString& tip) { self.setToolTip(tip); }>;

struct MoveCenter : Token {
    auto apply(QWidget& self) const noexcept -> void {
        const auto screen = self.screen();

        const auto screen_geometry = screen->availableGeometry();
        const auto screen_width    = screen_geometry.width();
        const auto screen_height   = screen_geometry.height();

        const auto widget_geometry = self.geometry();
        const auto widget_width    = widget_geometry.width();
        const auto widget_height   = widget_geometry.height();

        const auto x = (screen_width - widget_width) / 2;
        const auto y = (screen_height - widget_height) / 2;

        self.move(x, y);
    }
};

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

/// @note 该属性本质是转发构造，有 new 的行为
template <class T>
struct Layout : Token {
    T* layout_;

    explicit Layout(T* pointer) noexcept
        requires std::convertible_to<decltype(pointer), QLayout*>
        : layout_ { pointer } { }

    explicit Layout(auto&&... args)
        requires std::constructible_from<T, decltype(args)...>
        : layout_ { new T { std::forward<decltype(args)>(args)... } } { }

    void apply(QWidget& widget) const { widget.setLayout(layout_); }
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
template <class Final>
struct Bind : Token {
    Final*& widget;
    explicit Bind(Final*& widget) noexcept
        requires std::is_pointer<Final*>::value
        : widget(widget) { }
    void apply(Final& self) const noexcept { widget = &self; }
};

// 传入一个方法用来辅助构造，在没有想要的接口时用这个吧
template <typename Lambda>
struct Apply : Token {
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

CREEPER_DEFINE_CHECKER(trait);
}
