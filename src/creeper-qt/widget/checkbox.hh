#pragma once

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
///
/// @note 规格来源：
///   - https://m3.material.io/components/checkbox/specs
///   -
///   https://github.com/material-components/material-components-android/blob/master/lib/java/com/google/android/material/checkbox/MaterialCheckBox.java
class Checkbox : public QAbstractButton, public DSL {
    Q_OBJECT
    CREEPER_PIMPL_DEFINITION(Checkbox)

public:
    /// @brief 勾选状态
    enum class CheckState {
        UNSELECTED,    ///< 未选中
        SELECTED,      ///< 已选中
        INDETERMINATE, ///< 半选
    };

    /// @brief 尺寸度量
    /// @note 规格来源：
    ///   https://github.com/material-components/material-components-android/blob/master/lib/java/com/google/android/material/checkbox/res/values/styles.xml
    struct Measurements {
        double container_size    = 18; ///< Checkbox container size
        double container_shape   = 2;  ///< Checkbox container shape
        double icon_size         = 18; ///< Checkbox icon size
        double state_layer_size  = 40; ///< State layer size
        double stroke_width      = 2;  ///< CheckboxDefaults.StrokeWidth
        double touch_target_size = 48; ///< 最小可交互尺寸

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
        };
        struct CheckStateTokens {
            Tokens checked;
            Tokens unchecked;
            Tokens indeterminate;
        };

        CheckStateTokens enabled;
        CheckStateTokens disabled;
        CheckStateTokens error;
    };

    struct Defaults {
        constexpr static int kMinHeight = 40;

        static auto mapColors(const ColorScheme&, Colors&) -> void;
    };

    /// @brief 声明式构造
    explicit Checkbox(auto&&... props)
        : Checkbox {} {
        construct_with(std::forward<decltype(props)>(props)...);
    }

    auto loadColorScheme(const ColorScheme& scheme) -> void;
    auto bindThemeManager(ThemeManager& manager) -> void;

    auto setMeasurements(const Measurements& measurements) -> void;
    auto setColorTokens(const Colors& specs) -> void;

    auto checkState() const -> CheckState;
    auto setCheckState(CheckState state) -> void;

    auto setChecked(bool on) -> void;
    auto checked() const -> bool;

    auto setDisabled(bool on) -> void;
    auto disabled() const -> bool;

    auto setError(bool on) -> void;
    auto error() const -> bool;

Q_SIGNALS:
    /// @brief 勾选状态变化
    /// @param state 变化后的状态
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

/// @brief Checkbox 的声明式属性入口
namespace checkbox::pro {
    template <typename F>
    using OnCheckStateChanged = api::helper::SignalInjection<F, &Checkbox::checkStateChanged>;

    using CheckState   = ForwardProp<&Checkbox::setCheckState>;
    using Measurements = Checkbox::Measurements;
    using ColorSpecs   = Checkbox::Colors;

    using Error = ForwardProp<&Checkbox::setError>;

    using api::pro::Checked;
    using api::pro::Clickable;
    using api::pro::Disabled;

    using namespace api::scope::common;
    using namespace api::scope::theme;
    using namespace api::scope::widget;
}

}
