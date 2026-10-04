#pragma once

#include <concepts>
#include <qbitmap.h>
#include <qfont.h>
#include <qgraphicseffect.h>
#include <qicon.h>
#include <qpalette.h>
#include <qregion.h>
#include <qscreen.h>
#include <qsize.h>
#include <qwidget.h>
#include <type_traits>

namespace creeper::widget::pro {

// ============================================================================
// 基础尺寸属性 - 直接调用 QWidget 方法
// ============================================================================

struct MinimumWidth {
    int value;

    constexpr explicit MinimumWidth(int v) noexcept
        : value { v } { }

    friend auto dsl_invoke(QWidget& widget, const MinimumWidth& prop) -> void {
        widget.setMinimumWidth(prop.value);
    }
};

struct MaximumWidth {
    int value;

    constexpr explicit MaximumWidth(int v) noexcept
        : value { v } { }

    friend auto dsl_invoke(QWidget& widget, const MaximumWidth& prop) -> void {
        widget.setMaximumWidth(prop.value);
    }
};

struct FixedWidth {
    int value;

    constexpr explicit FixedWidth(int v) noexcept
        : value { v } { }

    friend auto dsl_invoke(QWidget& widget, const FixedWidth& prop) -> void {
        widget.setFixedWidth(prop.value);
    }
};

struct MinimumHeight {
    int value;

    constexpr explicit MinimumHeight(int v) noexcept
        : value { v } { }

    friend auto dsl_invoke(QWidget& widget, const MinimumHeight& prop) -> void {
        widget.setMinimumHeight(prop.value);
    }
};

struct MaximumHeight {
    int value;

    constexpr explicit MaximumHeight(int v) noexcept
        : value { v } { }

    friend auto dsl_invoke(QWidget& widget, const MaximumHeight& prop) -> void {
        widget.setMaximumHeight(prop.value);
    }
};

struct FixedHeight {
    int value;

    constexpr explicit FixedHeight(int v) noexcept
        : value { v } { }

    friend auto dsl_invoke(QWidget& widget, const FixedHeight& prop) -> void {
        widget.setFixedHeight(prop.value);
    }
};

// ============================================================================
// 布局与外观属性
// ============================================================================

struct LayoutDirection {
    Qt::LayoutDirection value;

    explicit LayoutDirection(Qt::LayoutDirection v) noexcept
        : value { v } { }

    friend auto dsl_invoke(QWidget& widget, const LayoutDirection& prop) -> void {
        widget.setLayoutDirection(prop.value);
    }
};

struct BackgroundRole {
    QPalette::ColorRole value;

    explicit BackgroundRole(QPalette::ColorRole v) noexcept
        : value { v } { }

    friend auto dsl_invoke(QWidget& widget, const BackgroundRole& prop) -> void {
        widget.setBackgroundRole(prop.value);
    }
};

struct ForegroundRole {
    QPalette::ColorRole value;

    explicit ForegroundRole(QPalette::ColorRole v) noexcept
        : value { v } { }

    friend auto dsl_invoke(QWidget& widget, const ForegroundRole& prop) -> void {
        widget.setForegroundRole(prop.value);
    }
};

// ============================================================================
// 遮罩与图形效果
// ============================================================================

struct ClearMask {
    explicit ClearMask() noexcept = default;

    friend auto dsl_invoke(QWidget& widget, const ClearMask&) -> void { widget.clearMask(); }
};

struct GraphicsEffect {
    QGraphicsEffect* value;

    explicit GraphicsEffect(QGraphicsEffect* v) noexcept
        : value { v } { }

    friend auto dsl_invoke(QWidget& widget, const GraphicsEffect& prop) -> void {
        widget.setGraphicsEffect(prop.value);
    }
};

// ============================================================================
// 窗口属性
// ============================================================================

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

struct WindowFlags {
    Qt::WindowFlags value;

    explicit WindowFlags(Qt::WindowFlags v) noexcept
        : value { v } { }

    friend auto dsl_invoke(QWidget& widget, const WindowFlags& prop) -> void {
        widget.setWindowFlags(prop.value);
    }
};

struct WindowOpacity {
    double value;

    constexpr explicit WindowOpacity(double v) noexcept
        : value { v } { }

    friend auto dsl_invoke(QWidget& widget, const WindowOpacity& prop) -> void {
        widget.setWindowOpacity(prop.value);
    }
};

// ============================================================================
// 父子关系
// ============================================================================

struct Parent {
    QWidget* value;

    explicit Parent(QWidget* v) noexcept
        : value { v } { }

    friend auto dsl_invoke(QWidget& widget, const Parent& prop) -> void {
        widget.setParent(prop.value);
    }
};

/// @brief 委托构造子组件并将其 parent 设为当前组件
///
/// 适用于弹窗、菜单等需要 QObject parent 提供生命周期与定位上下文、
/// 但不进入布局的组件，例如把 DropdownMenu 声明式地锚定到按钮：
///
/// @code
///     lnpro::Item<FilledButton> {
///         fbp::Child<DropdownMenu> {
///             dmp::Item<DropdownMenuItem> { ... },
///         },
///     }
/// @endcode
///
/// @note 该属性本质是转发构造，有 new 的行为
template <class T>
    requires std::derived_from<T, QWidget>
struct Child {
    T* child_pointer = nullptr;

    explicit Child(T* pointer) noexcept
        : child_pointer { pointer } { }

    explicit Child(auto&&... args) noexcept
        requires std::constructible_from<T, decltype(args)...>
        : child_pointer { new T { std::forward<decltype(args)>(args)... } } { }

    friend auto dsl_invoke(QWidget& widget, const Child& prop) -> void {
        prop.child_pointer->setParent(&widget, prop.child_pointer->windowFlags());
    }
};

// ============================================================================
// 尺寸属性 - 继承 QSize
// ============================================================================

struct MinimumSize : public QSize {
    using QSize::QSize;
    MinimumSize(const QSize& size)
        : QSize(size) { }

    friend auto dsl_invoke(QWidget& widget, const MinimumSize& prop) -> void {
        widget.setMinimumSize(static_cast<const QSize&>(prop));
    }
};

struct MaximumSize : public QSize {
    using QSize::QSize;
    MaximumSize(const QSize& size)
        : QSize(size) { }

    friend auto dsl_invoke(QWidget& widget, const MaximumSize& prop) -> void {
        widget.setMaximumSize(static_cast<const QSize&>(prop));
    }
};

struct SizeIncrement : public QSize {
    using QSize::QSize;
    SizeIncrement(const QSize& size)
        : QSize(size) { }

    friend auto dsl_invoke(QWidget& widget, const SizeIncrement& prop) -> void {
        widget.setSizeIncrement(static_cast<const QSize&>(prop));
    }
};

struct BaseSize : public QSize {
    using QSize::QSize;
    BaseSize(const QSize& size)
        : QSize(size) { }

    friend auto dsl_invoke(QWidget& widget, const BaseSize& prop) -> void {
        widget.setBaseSize(static_cast<const QSize&>(prop));
    }
};

struct FixedSize : public QSize {
    using QSize::QSize;
    FixedSize(const QSize& size)
        : QSize(size) { }

    friend auto dsl_invoke(QWidget& widget, const FixedSize& prop) -> void {
        widget.setFixedSize(static_cast<const QSize&>(prop));
    }
};

// ============================================================================
// 字体与外观
// ============================================================================

struct Font : public QFont {
    using QFont::QFont;
    Font(const QFont& font)
        : QFont(font) { }

    friend auto dsl_invoke(QWidget& widget, const Font& prop) -> void {
        widget.setFont(static_cast<const QFont&>(prop));
    }
};

struct BitmapMask : public QBitmap {
    using QBitmap::QBitmap;
    BitmapMask(const QBitmap& bitmap)
        : QBitmap(bitmap) { }

    friend auto dsl_invoke(QWidget& widget, const BitmapMask& prop) -> void {
        widget.setMask(static_cast<const QBitmap&>(prop));
    }
};

struct RegionMask : public QRegion {
    using QRegion::QRegion;
    RegionMask(const QRegion& region)
        : QRegion(region) { }

    friend auto dsl_invoke(QWidget& widget, const RegionMask& prop) -> void {
        widget.setMask(static_cast<const QRegion&>(prop));
    }
};

struct WindowIcon : public QIcon {
    using QIcon::QIcon;
    WindowIcon(const QIcon& icon)
        : QIcon(icon) { }

    friend auto dsl_invoke(QWidget& widget, const WindowIcon& prop) -> void {
        widget.setWindowIcon(static_cast<const QIcon&>(prop));
    }
};

struct WindowIconText : public QString {
    using QString::QString;
    WindowIconText(const QString& text)
        : QString(text) { }

    friend auto dsl_invoke(QWidget& widget, const WindowIconText& prop) -> void {
        widget.setWindowIconText(static_cast<const QString&>(prop));
    }
};

struct WindowRole : public QString {
    using QString::QString;
    WindowRole(const QString& role)
        : QString(role) { }

    friend auto dsl_invoke(QWidget& widget, const WindowRole& prop) -> void {
        widget.setWindowRole(static_cast<const QString&>(prop));
    }
};

struct WindowFilePath : public QString {
    using QString::QString;
    WindowFilePath(const QString& path)
        : QString(path) { }

    friend auto dsl_invoke(QWidget& widget, const WindowFilePath& prop) -> void {
        widget.setWindowFilePath(static_cast<const QString&>(prop));
    }
};

struct ToolTip : public QString {
    using QString::QString;
    ToolTip(const QString& tip)
        : QString(tip) { }

    friend auto dsl_invoke(QWidget& widget, const ToolTip& prop) -> void {
        widget.setToolTip(static_cast<const QString&>(prop));
    }
};

// ============================================================================
// 特殊行为属性
// ============================================================================

/// 将窗口移动到屏幕中心
struct MoveCenter {
    friend auto dsl_invoke(QWidget& widget, const MoveCenter&) -> void {
        const auto screen = widget.screen();

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

/// 设置尺寸策略
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

// ============================================================================
// 布局属性
// ============================================================================

/// @note 该属性本质是转发构造，有 new 的行为
template <class T>
    requires std::convertible_to<T*, QLayout*>
struct Layout {
    T* layout_;

    explicit Layout(T* pointer) noexcept
        : layout_ { pointer } { }

    explicit Layout(auto&&... args)
        requires std::constructible_from<T, decltype(args)...>
        : layout_ { new T { std::forward<decltype(args)>(args)... } } { }

    explicit Layout(auto&&... args) noexcept {
        // 实例化一次错误的构造，让错误爆出来
        T { std::forward<decltype(args)>(args)... };
    }

    friend auto dsl_invoke(QWidget& widget, const Layout& prop) -> void {
        widget.setLayout(prop.layout_);
    }
};

// ============================================================================
// 指针绑定与自定义逻辑
// ============================================================================

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
struct Bind {
    Final*& widget;

    explicit Bind(Final*& widget) noexcept
        requires std::is_pointer_v<Final*>
        : widget(widget) { }

    friend auto dsl_invoke(Final& self, const Bind& prop) -> void { prop.widget = &self; }
};

/// 传入一个方法用来辅助构造，在没有想要的接口时用这个吧
template <typename Lambda>
struct Apply {
    Lambda lambda;

    explicit Apply(Lambda lambda) noexcept
        : lambda { std::move(lambda) } { }

    friend auto dsl_invoke(auto& widget, const Apply& prop) -> void {
        if constexpr (std::invocable<Lambda>) prop.lambda();
        else if constexpr (std::invocable<Lambda, decltype(widget)>) prop.lambda(widget);
    }
};

} // namespace creeper::widget::pro
