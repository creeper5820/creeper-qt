#pragma once

#include "creeper-qt/details/token/snackbar/defaults.hh"
#include "creeper-qt/details/token/snackbar/measurements.hh"
#include "creeper-qt/utility/api/helper/signal-injection.hh" // IWYU pragma: keep
#include "creeper-qt/utility/api/pro/text.hh"                // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/common.hh"            // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/theme.hh"             // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/widget.hh"            // IWYU pragma: keep
#include "creeper-qt/utility/qt_wrapper/enter-event.hh"
#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/utility/wrapper/forward_prop.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"

#include <qwidget.h>

namespace creeper {

enum class SnackbarResult { DISMISSED, ACTION_PERFORMED };

/// @brief Material Design 3 Snackbar
///
/// 对应 Compose 的 `Snackbar(snackbarData)`：正文、可选的 action 文本按钮与关闭按钮，
/// 外层带 `snackbar_padding` 外边距；只负责外观与交互，不负责显示时长与进出场。
class Snackbar : public QWidget, public DSL {
    Q_OBJECT
    CREEPER_PIMPL_DEFINITION(Snackbar)

public:
    using Measurements = token::snackbar::Measurements;
    using Colors       = token::snackbar::Colors;
    using Defaults     = token::snackbar::Defaults;

    explicit Snackbar(auto&&... props)
        : Snackbar { } {
        construct_with(std::forward<decltype(props)>(props)...);
    }

    auto loadColorScheme(const ColorScheme& scheme) -> void;
    auto bindThemeManager(ThemeManager& manager) -> void;

    auto setMeasurements(const Measurements&) -> void;
    auto setColors(const Colors&) -> void;

    auto setText(const QString& text) -> void;
    auto text() const -> QString;

    /// @brief action 按钮文本，空字符串表示没有 action
    auto setActionLabel(const QString& label) -> void;
    auto actionLabel() const -> QString;

    auto setWithDismissAction(bool on) -> void;
    auto withDismissAction() const -> bool;

    auto setActionOnNewLine(bool on) -> void;
    auto actionOnNewLine() const -> bool;

    auto sizeHint() const -> QSize override;
    auto minimumSizeHint() const -> QSize override;
    auto hasHeightForWidth() const -> bool override;
    auto heightForWidth(int width) const -> int override;

Q_SIGNALS:
    auto actionPerformed() -> void;
    auto dismissed() -> void;

protected:
    auto resizeEvent(QResizeEvent* event) -> void override;
    auto changeEvent(QEvent* event) -> void override;

    auto enterEvent(qt::EnterEvent* event) -> void override;
    auto leaveEvent(QEvent* event) -> void override;
    auto mouseMoveEvent(QMouseEvent* event) -> void override;
    auto mousePressEvent(QMouseEvent* event) -> void override;
    auto mouseReleaseEvent(QMouseEvent* event) -> void override;

    auto focusInEvent(QFocusEvent* event) -> void override;
    auto focusOutEvent(QFocusEvent* event) -> void override;
    auto focusNextPrevChild(bool next) -> bool override;
    auto keyPressEvent(QKeyEvent* event) -> void override;
    auto keyReleaseEvent(QKeyEvent* event) -> void override;

    auto paintEvent(QPaintEvent* event) -> void override;
};

namespace snackbar::pro {
    template <typename F>
    using OnActionPerformed = api::helper::SignalInjection<F, &Snackbar::actionPerformed>;
    template <typename F>
    using OnDismissed = api::helper::SignalInjection<F, &Snackbar::dismissed>;

    using ActionLabel       = ForwardProp<&Snackbar::setActionLabel>;
    using WithDismissAction = ForwardProp<&Snackbar::setWithDismissAction>;
    using ActionOnNewLine   = ForwardProp<&Snackbar::setActionOnNewLine>;
    using Measurements      = Snackbar::Measurements;
    using Colors            = Snackbar::Colors;

    using api::pro::Text;

    using namespace api::scope::common;
    using namespace api::scope::theme;
    using namespace api::scope::widget;
}

}
