#pragma once

#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"
#include "creeper-qt/utility/wrapper/property.hh"
#include "creeper-qt/utility/wrapper/widget.hh"

#include <qwidget.h>

namespace creeper::circular_progress_indicator::details {

class CircularProgressIndicator : public QWidget {
    CREEPER_PIMPL_DEFINITION(CircularProgressIndicator);

public:
    void set_color_scheme(const ColorScheme&);
    void load_theme_manager(ThemeManager&);

    void set_progress(double) noexcept;
    double progress() const noexcept;

    void set_indeterminate(bool) noexcept;
    bool indeterminate() const noexcept;

    void set_indicator_color(const QColor&) noexcept;
    void set_track_color(const QColor&) noexcept;

    /// @note 线宽为 0 时按直径的 10% 自适应
    void set_stroke_width(double) noexcept;

protected:
    void paintEvent(QPaintEvent*) override;
};

}
namespace creeper::circular_progress_indicator::pro {

using Token = creeper::Token<details::CircularProgressIndicator>;

using Progress = SetterProp<Token, double, [](auto& self, double v) { self.set_progress(v); }>;

using Indeterminate =
    SetterProp<Token, bool, [](auto& self, bool v) { self.set_indeterminate(v); }>;

using IndicatorColor =
    SetterProp<Token, QColor, [](auto& self, const QColor& v) { self.set_indicator_color(v); }>;

using TrackColor =
    SetterProp<Token, QColor, [](auto& self, const QColor& v) { self.set_track_color(v); }>;

using StrokeWidth =
    SetterProp<Token, double, [](auto& self, double v) { self.set_stroke_width(v); }>;

using namespace theme::pro;
using namespace widget::pro;
}
namespace creeper {

using CircularProgressIndicator =
    Declarative<circular_progress_indicator::details::CircularProgressIndicator,
        TokenOr<circular_progress_indicator::pro::Token, widget::pro::Token, theme::pro::Token>>;

}
