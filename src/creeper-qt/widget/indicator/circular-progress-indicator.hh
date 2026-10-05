#pragma once

#include "creeper-qt/utility/api/scope/common.hh" // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/theme.hh"  // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/widget.hh" // IWYU pragma: keep
#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/utility/wrapper/forward_prop.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"

#include <qwidget.h>

namespace creeper {

class CircularProgressIndicator : public QWidget, public DSL {
    CREEPER_PIMPL_DEFINITION(CircularProgressIndicator);

public:
    explicit CircularProgressIndicator(auto&&... props)
        : CircularProgressIndicator { } {
        construct_with(std::forward<decltype(props)>(props)...);
    }

    auto loadColorScheme(const ColorScheme&) -> void;
    auto bindThemeManager(ThemeManager&) -> void;

    auto setProgress(double) noexcept -> void;
    auto progress() const noexcept -> double;

    auto setIndeterminate(bool) noexcept -> void;
    auto indeterminate() const noexcept -> bool;

    auto setIndicatorColor(const QColor&) noexcept -> void;
    auto setTrackColor(const QColor&) noexcept -> void;

    /// @note 线宽为 0 时按直径的 10% 自适应
    auto setStrokeWidth(double) noexcept -> void;

protected:
    auto paintEvent(QPaintEvent*) -> void override;
};

namespace circular_progress_indicator::pro {
    using Progress       = ForwardProp<&CircularProgressIndicator::setProgress>;
    using Indeterminate  = ForwardProp<&CircularProgressIndicator::setIndeterminate>;
    using IndicatorColor = ForwardProp<&CircularProgressIndicator::setIndicatorColor>;
    using TrackColor     = ForwardProp<&CircularProgressIndicator::setTrackColor>;
    using StrokeWidth    = ForwardProp<&CircularProgressIndicator::setStrokeWidth>;

    using namespace api::scope::common;
    using namespace api::scope::theme;
    using namespace api::scope::widget;
}

}
