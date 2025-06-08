#pragma once

#include "utility/wrapper/pimpl.hh"
#include "utility/wrapper/property.hh"
#include "widget/button/button.hh"
#include "widget/widget.hh"

#include <qpushbutton.h>

namespace creeper {

namespace internal {

    class FilledButton : public QPushButton {
        CREEPER_PIMPL_DEFINTION(FilledButton);

    public:
        void set_radius(double radius);
        void set_border_width(double border);

        void set_water_color(const QColor& color);
        void set_border_color(const QColor& color);
        void set_text_color(const QColor& color);
        void set_background_color(const QColor& color);

        void set_water_ripple_status(bool enable);
        void set_water_ripple_step(double step);

    protected:
        // void mouseReleaseEvent(QMouseEvent* event) override;
        // void paintEvent(QPaintEvent* event) override;
        // void enterEvent(QEvent* event) override;
        // void leaveEvent(QEvent* event) override;
    };

} // namespace internal

namespace pro::filled_button {

    using Property = InternalProperty<internal::FilledButton>;

    template <typename T>
    concept property_concept = std::derived_from<T, Property> || pro::widget::property_concept<T>;

    using Text = common::Text<internal::FilledButton, Property>;

    using Radius = common::Radius<internal::FilledButton, Property>;
    using Border = common::BorderWidth<internal::FilledButton, Property>;

    using Clickable = button::Clickable<internal::FilledButton, Property>;

    using namespace pro::widget;
} // namespace pro::filled_button

class FilledButton : public internal::FilledButton {
    CREEPER_DEFINE_CONSTROCTOR(FilledButton, pro::filled_button);
    using internal::FilledButton::FilledButton;
};

} // namespace creeper
