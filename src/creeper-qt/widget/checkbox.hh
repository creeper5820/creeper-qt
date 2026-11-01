#pragma once

#include "creeper-qt/details/token/checkbox/defaults.hh"
#include "creeper-qt/details/token/checkbox/measurements.hh"
#include "creeper-qt/utility/api/helper/signal-injection.hh" // IWYU pragma: keep
#include "creeper-qt/utility/api/pro/checked.hh"             // IWYU pragma: keep
#include "creeper-qt/utility/api/pro/clickable.hh"           // IWYU pragma: keep
#include "creeper-qt/utility/api/pro/disabled.hh"            // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/common.hh"            // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/theme.hh"             // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/widget.hh"            // IWYU pragma: keep
#include "creeper-qt/utility/qt_wrapper/enter-event.hh"
#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/utility/wrapper/forward_prop.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"

#include <qabstractbutton.h>

namespace creeper {

/// @brief Material Design 3 复选框
///
/// 支持未选 / 选中 / 半选三态，可禁用、可配颜色。
class Checkbox : public QAbstractButton, public DSL {
    Q_OBJECT
    CREEPER_PIMPL_DEFINITION(Checkbox)

public:
    enum class CheckState {
        UNSELECTED,    ///< 未选中
        SELECTED,      ///< 已选中
        INDETERMINATE, ///< 半选
    };

    using Measurements = token::checkbox::Measurements;
    using Colors       = token::checkbox::Colors;
    using Defaults     = token::checkbox::Defaults;

    explicit Checkbox(auto&&... props)
        : Checkbox { } {
        construct_with(std::forward<decltype(props)>(props)...);
    }

    auto loadColorScheme(const ColorScheme& scheme) -> void;
    auto bindThemeManager(ThemeManager& manager) -> void;

    auto setMeasurements(const Measurements&) -> void;
    auto setColors(const Colors&) -> void;

    auto sizeHint() const -> QSize override;

    auto checkState() const -> CheckState;
    auto setCheckState(CheckState state) -> void;

    auto setChecked(bool on) -> void;
    auto checked() const -> bool;

    auto setDisabled(bool on) -> void;
    auto disabled() const -> bool;

    auto setError(bool on) -> void;
    auto error() const -> bool;

Q_SIGNALS:
    auto checkStateChanged(CheckState state) -> void;

protected:
    auto enterEvent(qt::EnterEvent* event) -> void override;
    auto leaveEvent(QEvent* event) -> void override;

    auto focusInEvent(QFocusEvent* event) -> void override;
    auto focusOutEvent(QFocusEvent* event) -> void override;

    auto mousePressEvent(QMouseEvent* event) -> void override;
    auto mouseReleaseEvent(QMouseEvent* event) -> void override;

    auto paintEvent(QPaintEvent* event) -> void override;
};

namespace checkbox::pro {
    template <typename F>
    using OnCheckStateChanged = api::helper::SignalInjection<F, &Checkbox::checkStateChanged>;

    using CheckState   = ForwardProp<&Checkbox::setCheckState>;
    using Error        = ForwardProp<&Checkbox::setError>;
    using Measurements = Checkbox::Measurements;
    using Colors       = Checkbox::Colors;

    using api::pro::Checked;
    using api::pro::Clickable;
    using api::pro::Disabled;

    using namespace api::scope::common;
    using namespace api::scope::theme;
    using namespace api::scope::widget;
}

}
