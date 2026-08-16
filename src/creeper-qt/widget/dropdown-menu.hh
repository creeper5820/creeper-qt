#pragma once

#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/trait/widget.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"
#include "creeper-qt/utility/wrapper/property.hh"
#include "creeper-qt/utility/wrapper/widget.hh"

namespace creeper::dropdown_menu::details {

/// Material 3 DropdownMenu，纯弹出式菜单。
///
/// 菜单显示在独立的弹出窗口中，自身不占据布局空间，通过 Anchor 锚定到
/// 其他组件上定位。可见性由 Expanded 受控：外部点击或 Esc 时发出
/// dismiss_requested()，由应用决定是否置回 false；点击菜单项不会自动
/// 关闭菜单。
class DropdownMenu : public QWidget {
    Q_OBJECT
    CREEPER_PIMPL_DEFINITION(DropdownMenu);

public:
    auto set_color_scheme(const ColorScheme&) -> void;

    auto load_theme_manager(ThemeManager&) -> void;

    /// 设置锚组件，菜单依据其全局位置定位，同时作为 QObject parent。
    auto set_anchor(QWidget*) -> void;
    auto anchor() const noexcept -> QWidget*;

    /// 受控展开状态；程序性关闭不会触发 dismiss_requested()。
    auto set_expanded(bool) -> void;
    auto expanded() const noexcept -> bool;

    /// 定位完成后叠加的偏移，RTL 布局下 x 方向取反。
    auto set_offset(QPoint) -> void;

    /// 覆盖容器颜色；传入无效 QColor 恢复主题默认。
    auto set_container_color(const QColor&) -> void;

    /// 容器圆角半径，默认 4（M3 extra-small）。
    auto set_corner_radius(double) -> void;

    /// 向内容列追加菜单项，通常为 DropdownMenuItem。
    auto add_item(QWidget*) -> void;
    auto content_count() const noexcept -> int;

Q_SIGNALS:
    /// 用户请求关闭菜单（点击菜单外部或按下 Esc）时发出。
    auto dismiss_requested() -> void;

protected:
    auto event(QEvent*) -> bool override;
    auto paintEvent(QPaintEvent*) -> void override;
    auto hideEvent(QHideEvent*) -> void override;
    auto eventFilter(QObject*, QEvent*) -> bool override;
    auto wheelEvent(QWheelEvent*) -> void override;
    auto keyPressEvent(QKeyEvent*) -> void override;
};

}

namespace creeper::dropdown_menu::pro {
using Token = creeper::Token<details::DropdownMenu>;

/// 受控展开状态，可配合 MutableForward<MutableBool> 使用
using Expanded = SetterProp<Token, bool, [](auto& self, bool value) { self.set_expanded(value); }>;

/// 锚组件，菜单依据其全局位置定位
struct Anchor : Token {
    QWidget* widget = nullptr;
    explicit Anchor(QWidget* widget) noexcept
        : widget { widget } { }
    auto apply(details::DropdownMenu& self) const -> void { self.set_anchor(widget); }
};

/// 定位偏移，RTL 布局下 x 方向取反
using Offset =
    DerivedProp<Token, QPoint, [](auto& self, const QPoint& value) { self.set_offset(value); }>;

/// 覆盖容器颜色，默认取自主题 surface_container
using ContainerColor = SetterProp<Token, QColor,
    [](auto& self, const QColor& value) { self.set_container_color(value); }>;

/// 容器圆角半径，默认 4
using CornerRadius =
    SetterProp<Token, double, [](auto& self, double value) { self.set_corner_radius(value); }>;

/// 用户请求关闭（外部点击 / Esc）时的回调
template <typename F>
using OnDismissRequest =
    common::pro::SignalInjection<F, Token, &details::DropdownMenu::dismiss_requested>;

/// 向菜单内容列追加内容项，通常为 DropdownMenuItem
template <item_trait T>
struct Item : Token {
    T* item_pointer = nullptr;

    explicit Item(T* pointer) noexcept
        : item_pointer { pointer } { }

    explicit Item(auto&&... args) noexcept
        requires std::constructible_from<T, decltype(args)...>
        : item_pointer { new T { std::forward<decltype(args)>(args)... } } { }

    auto apply(details::DropdownMenu& self) const { self.add_item(item_pointer); }
};

using namespace widget::pro;
using namespace theme::pro;
}

namespace creeper {

using DropdownMenu = Declarative<dropdown_menu::details::DropdownMenu,
    TokenOr<dropdown_menu::pro::Token, widget::pro::Token, theme::pro::Token>>;

}
