#pragma once

#include "creeper-qt/details/token/switch_/defaults.hh"
#include "creeper-qt/details/token/switch_/measurements.hh"
#include "creeper-qt/utility/api/pro/checked.hh"   // IWYU pragma: keep
#include "creeper-qt/utility/api/pro/clickable.hh" // IWYU pragma: keep
#include "creeper-qt/utility/api/pro/disabled.hh"  // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/common.hh"  // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/theme.hh"   // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/widget.hh"  // IWYU pragma: keep
#include "creeper-qt/utility/qt_wrapper/enter-event.hh"
#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"

#include <qabstractbutton.h>

namespace creeper {

class Switch : public QAbstractButton, public DSL {
    CREEPER_PIMPL_DEFINITION(Switch)

public:
    using Measurements = token::switch_::Measurements;
    using Colors       = token::switch_::Colors;
    using Defaults     = token::switch_::Defaults;

    explicit Switch(auto&&... props)
        : Switch { } {
        construct_with(std::forward<decltype(props)>(props)...);
    }

    auto loadColorScheme(const ColorScheme&) -> void;
    auto bindThemeManager(ThemeManager&) -> void;

    auto setMeasurements(const Measurements&) -> void;
    auto setColors(const Colors&) -> void;

    auto sizeHint() const -> QSize override;

    auto setDisabled(bool) -> void;
    auto disabled() const -> bool;

    auto setChecked(bool) -> void;
    auto checked() const -> bool;

protected:
    // 添加 Hover 动画
    auto enterEvent(qt::EnterEvent* event) -> void override;
    auto leaveEvent(QEvent* event) -> void override;

    // 添加 Focus 动画
    auto focusInEvent(QFocusEvent* event) -> void override;
    auto focusOutEvent(QFocusEvent* event) -> void override;

    // 添加 Press 动画
    auto mousePressEvent(QMouseEvent* event) -> void override;
    auto mouseReleaseEvent(QMouseEvent* event) -> void override;

    // 实现视觉效果
    auto paintEvent(QPaintEvent* event) -> void override;
};

namespace switch_::pro {
    using Measurements = Switch::Measurements;
    using Colors       = Switch::Colors;

    using api::pro::Checked;
    using api::pro::Clickable;
    using api::pro::Disabled;

    using namespace api::scope::common;
    using namespace api::scope::theme;
    using namespace api::scope::widget;
}
}
