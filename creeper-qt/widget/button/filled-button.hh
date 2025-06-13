#pragma once

#include "utility/wrapper/pimpl.hh"
#include "utility/wrapper/property.hh"
#include "widget/button/button.hh"
#include "widget/widget.hh"

#include <qpushbutton.h>

namespace creeper {
namespace filled_button::internal {

    class FilledButton : public QPushButton {
        CREEPER_PIMPL_DEFINTION(FilledButton);

    public:
        void set_radius(double radius);
        void set_border_width(double border);

        void set_water_color(const QColor& color);
        void set_border_color(const QColor& color);
        void set_text_color(const QColor& color);
        void set_background(const QColor& color);

        void set_water_ripple_status(bool enable);
        void set_water_ripple_step(double step);

    protected:
        void mouseReleaseEvent(QMouseEvent* event) override;
        void paintEvent(QPaintEvent* event) override;
        void enterEvent(QEvent* event) override;
        void leaveEvent(QEvent* event) override;
    };

}
namespace filled_button::pro {

    using Property = common::InternalProperty<internal::FilledButton>;

    template <typename T>
    concept property_concept = std::derived_from<T, Property> || widget::pro::property_concept<T>;

    using Text        = common::pro::Text<internal::FilledButton, Property>;
    using Radius      = common::pro::Radius<internal::FilledButton, Property>;
    using BorderWidth = common::pro::BorderWidth<internal::FilledButton, Property>;
    using BorderColor = common::pro::BorderColor<internal::FilledButton, Property>;
    using Background  = common::pro::Background<internal::FilledButton, Property>;
    using WaterColor  = common::pro::WaterColor<internal::FilledButton, Property>;
    using Clickable   = button::pro::Clickable<internal::FilledButton, Property>;

    using namespace widget::pro;
}

class FilledButton : public filled_button::internal::FilledButton {
    CREEPER_DEFINE_CONSTROCTOR(FilledButton, filled_button::pro);
    using filled_button::internal::FilledButton::FilledButton;
};

}
