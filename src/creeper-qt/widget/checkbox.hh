#pragma once

#include "creeper-qt/utility/api/helper/signal-injection.hh" // IWYU pragma: keep
#include "creeper-qt/utility/api/pro/checked.hh"             // IWYU pragma: keep
#include "creeper-qt/utility/api/pro/clickable.hh"           // IWYU pragma: keep
#include "creeper-qt/utility/api/pro/disabled.hh"            // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/common.hh"            // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/theme.hh"             // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/widget.hh"            // IWYU pragma: keep
#include "creeper-qt/utility/math/cubic-bezier.hh"
#include "creeper-qt/utility/qt_wrapper/enter-event.hh"
#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/utility/wrapper/forward_prop.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"

#include <qabstractbutton.h>

#include <chrono>

namespace creeper {

/// @brief Material Design 3 复选框
///
/// 支持未选 / 选中 / 半选三态，可禁用、可配颜色。
///
/// @note 规格来源：
///   - https://m3.material.io/components/checkbox/specs
///   -
///   https://github.com/material-components/material-components-android/blob/master/lib/java/com/google/android/material/checkbox/MaterialCheckBox.java
class Checkbox : public QAbstractButton, public DSL {
    Q_OBJECT
    CREEPER_PIMPL_DEFINITION(Checkbox)

public:
    enum class CheckState {
        UNSELECTED,    ///< 未选中
        SELECTED,      ///< 已选中
        INDETERMINATE, ///< 半选
    };

    /// @brief 尺寸度量
    /// @note 规格来源：
    ///   https://github.com/material-components/material-components-android/blob/master/lib/java/com/google/android/material/checkbox/res/values/styles.xml
    struct Measurements {
        int container_size    = Defaults::kContainerSize;
        int container_shape   = Defaults::kContainerShape;
        int icon_size         = Defaults::kIconSize;
        int state_layer_size  = Defaults::kStateLayerSize;
        int stroke_width      = Defaults::kStrokeWidth;
        int touch_target_size = Defaults::kTouchTargetSize;

        friend auto dsl_invoke(Checkbox& self, const Measurements& m) -> void {
            self.setMeasurements(m);
        }
    };

    /// @brief 颜色规格，按状态分组
    /// @note 规格来源：Compose Material3 CheckboxDefaults.defaultCheckboxColors
    struct Colors {
        struct Tokens {
            QColor checkmark;
            QColor box;
            QColor border;
            QColor state_layer;
        };
        struct CheckStateTokens {
            Tokens checked;
            Tokens unchecked;
            Tokens indeterminate;
        };

        CheckStateTokens enabled;
        CheckStateTokens disabled;
        CheckStateTokens error;

        QColor focus_ring;

        friend auto dsl_invoke(Checkbox& self, const Colors& colors) -> void {
            self.setColors(colors);
        }
    };

    struct Defaults {
        constexpr static auto kContainerSize   = 18;
        constexpr static auto kContainerShape  = 2;
        constexpr static auto kIconSize        = 18;
        constexpr static auto kStateLayerSize  = 40;
        constexpr static auto kStrokeWidth     = 2;
        constexpr static auto kTouchTargetSize = 48;

        constexpr static auto kSplashRadius   = 20;
        constexpr static auto kFocusRingWidth = 3;

        constexpr static auto kToggleDuration   = std::chrono::milliseconds { 200 };
        constexpr static auto kReactionDuration = std::chrono::milliseconds { 200 };
        constexpr static auto kReactionFade     = std::chrono::milliseconds { 50 };

        constexpr static auto kRadialReactionAlpha = 0x1F / 255.0;

        constexpr static auto kEaseIn        = CubicBezierSolution { 0.42, 0.0, 1.00, 1.0 };
        constexpr static auto kEaseOut       = CubicBezierSolution { 0.00, 0.0, 0.58, 1.0 };
        constexpr static auto kFastOutSlowIn = CubicBezierSolution { 0.40, 0.0, 0.20, 1.0 };

        static auto mapColors(const ColorScheme&, Colors&) -> void;
    };

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
    using Measurements = Checkbox::Measurements;
    using Colors       = Checkbox::Colors;

    using Error = ForwardProp<&Checkbox::setError>;

    using api::pro::Checked;
    using api::pro::Clickable;
    using api::pro::Disabled;

    using namespace api::scope::common;
    using namespace api::scope::theme;
    using namespace api::scope::widget;
}

}
