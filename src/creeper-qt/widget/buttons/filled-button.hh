#pragma once

#include "creeper-qt/utility/qt_wrapper/enter-event.hh"
#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"
#include "creeper-qt/utility/wrapper/property.hh"
#include "creeper-qt/utility/wrapper/widget.hh"
#include "creeper-qt/widget/buttons/button.hh"
#include "qabstractbutton.h"

namespace creeper::filled_button::internal {

class FilledButton : public QAbstractButton {
    CREEPER_PIMPL_DEFINITION(FilledButton);

public:
    void set_color_scheme(const ColorScheme& pack);
    void load_theme_manager(ThemeManager& manager);

    void set_radius(double radius);
    void set_border_width(double border);

    void set_water_color(const QColor& color);
    void set_border_color(const QColor& color);
    void set_text_color(const QColor& color);
    void set_background(const QColor& color);
    void set_hover_color(const QColor& color);

    void set_water_ripple_status(bool enable);
    void set_water_ripple_step(double step);

protected:
    void mouseReleaseEvent(QMouseEvent* event) override;

    void enterEvent(qt::EnterEvent* event) override;
    void leaveEvent(QEvent* event) override;

    void paintEvent(QPaintEvent* event) override;
};

}
namespace creeper::filled_button::pro {

using namespace button::pro;
using namespace widget::pro;
using namespace theme::pro;

}
namespace creeper {

using FilledButton = Declarative<filled_button::internal::FilledButton,
    CheckerOr<button::pro::checker, widget::pro::checker, theme::pro::checker>>;

}
