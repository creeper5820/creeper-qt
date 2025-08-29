#pragma once
#include "filled-button.hh"

namespace creeper::text_button::internal {
class TextButton : public FilledButton {
public:
    void set_color_scheme(const ColorScheme& color_scheme) {
        set_background(Qt::transparent);
        set_text_color(color_scheme.primary);

        auto hover_color = color_scheme.primary;
        hover_color.setAlphaF(0.08);
        set_hover_color(hover_color);

        auto water_color = QColor {};
        if (color_scheme.primary.lightness() > 128) {
            water_color = color_scheme.primary.darker(130);
        } else {
            water_color = color_scheme.primary.lighter(130);
        }
        water_color.setAlphaF(0.25);
        set_water_color(water_color);

        update();
    }

    void load_theme_manager(ThemeManager& manager) {
        manager.append_handler(this,
            [this](const ThemeManager& manager) { set_color_scheme(manager.color_scheme()); });
    }
};
}

namespace creeper {

namespace text_button::pro {
    using namespace filled_button::pro;
}

using TextButton = Declarative<text_button::internal::TextButton, FilledButton::Checker>;
}
