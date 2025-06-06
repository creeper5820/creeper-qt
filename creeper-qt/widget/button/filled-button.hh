#pragma once

#include "utility/wrapper/pimpl.hh"
#include "utility/wrapper/property.hh"
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
    };

} // namespace internal

namespace pro::filled_button {

    using Property = InternalProperty<internal::FilledButton>;

    template <typename T>
    concept property_concept = std::is_base_of_v<Property, T> || pro::widget::property_concept<T>;

    using Radius = common::Radius<internal::FilledButton, Property>;
    using Border = common::BorderWidth<internal::FilledButton, Property>;

    // QPushButton
    using Text = common::Text<internal::FilledButton, Property>;

    using namespace pro::widget;

} // namespace pro::filled_button

class FilledButton : public internal::FilledButton {
    CREEPER_DEFINE_CONSTROCTOR(FilledButton, pro::filled_button);
    using internal::FilledButton::FilledButton;
};

} // namespace creeper
