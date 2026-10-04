#pragma once

#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/trait/widget.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/utility/wrapper/forward_prop.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"
#include "creeper-qt/utility/wrapper/widget.hh"

namespace creeper {

/// Material 3 DropdownMenu，纯弹出式菜单。
///
/// 菜单显示在独立的弹出窗口中，自身不占据布局空间，通过 Anchor 锚定到
/// 其他组件上定位。可见性由 Expanded 受控：外部点击或 Esc 时发出
/// dismissRequested()，由应用决定是否置回 false；点击菜单项不会自动
/// 关闭菜单。
class DropdownMenu : public QWidget, public DSL {
    Q_OBJECT
    CREEPER_PIMPL_DEFINITION(DropdownMenu);

public:
    using QWidget::QWidget;

    explicit DropdownMenu(auto&&... props)
        : DropdownMenu { } {
        construct_with(std::forward<decltype(props)>(props)...);
    }

    auto loadColorScheme(const ColorScheme&) -> void;

    auto bindThemeManager(ThemeManager&) -> void;

    /// 设置锚组件，菜单依据其全局位置定位，同时作为 QObject parent。
    auto setAnchor(QWidget*) -> void;
    auto anchor() const noexcept -> QWidget*;

    /// 受控展开状态；程序性关闭不会触发 dismissRequested()。
    auto setExpanded(bool) -> void;
    auto expanded() const noexcept -> bool;

    /// 定位完成后叠加的偏移，RTL 布局下 x 方向取反。
    auto setOffset(QPoint) -> void;

    /// 覆盖容器颜色；传入无效 QColor 恢复主题默认。
    auto setContainerColor(const QColor&) -> void;

    /// 容器圆角半径，默认 4（M3 extra-small）。
    auto setCornerRadius(double) -> void;

    /// 向内容列追加菜单项，通常为 DropdownMenuItem。
    auto addItem(QWidget*) -> void;
    auto contentCount() const noexcept -> int;

Q_SIGNALS:
    /// 用户请求关闭菜单（点击菜单外部或按下 Esc）时发出。
    auto dismissRequested() -> void;

protected:
    auto event(QEvent*) -> bool override;
    auto paintEvent(QPaintEvent*) -> void override;
    auto hideEvent(QHideEvent*) -> void override;
    auto eventFilter(QObject*, QEvent*) -> bool override;
    auto wheelEvent(QWheelEvent*) -> void override;
    auto keyPressEvent(QKeyEvent*) -> void override;
};

namespace dropdown_menu::pro {
    using namespace common::pro;
    using namespace widget::pro;
    using namespace theme::pro;

    /// 受控展开状态，可配合 MutableForward<MutableBool> 使用
    using Expanded = ForwardProp<&DropdownMenu::setExpanded>;

    /// 锚组件，菜单依据其全局位置定位
    using Anchor = ForwardProp<&DropdownMenu::setAnchor>;

    /// 定位偏移，RTL 布局下 x 方向取反
    using Offset = ForwardProp<&DropdownMenu::setOffset>;

    /// 覆盖容器颜色，默认取自主题 surface_container
    using ContainerColor = ForwardProp<&DropdownMenu::setContainerColor>;

    /// 容器圆角半径，默认 4
    using CornerRadius = ForwardProp<&DropdownMenu::setCornerRadius>;

    /// 用户请求关闭（外部点击 / Esc）时的回调
    template <typename F>
    using OnDismissRequest = common::pro::SignalInjection<F, &DropdownMenu::dismissRequested>;

    /// 向菜单内容列追加内容项，通常为 DropdownMenuItem
    template <item_trait T>
    struct Item {
        T* item_pointer = nullptr;

        explicit Item(T* pointer) noexcept
            : item_pointer { pointer } { }

        explicit Item(auto&&... args) noexcept
            requires std::constructible_from<T, decltype(args)...>
            : item_pointer { new T { std::forward<decltype(args)>(args)... } } { }

        friend auto dsl_invoke(DropdownMenu& self, const Item& prop) -> void {
            self.addItem(prop.item_pointer);
        }
    };

}

}
