#pragma once
#include "filled-button.hh"

namespace creeper {
namespace text_button::internal {
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
namespace text_button::pro {

    using Token = common::Token<internal::TextButton>;

    template <typename T>
    concept property_concept = std::derived_from<T, Token> //
        || button::pro::property_concept<T>                //
        || util::theme::pro::property_concept<T>;

    using namespace button::pro;
    using namespace util::theme::pro;
}
class TextButton : public text_button::internal::TextButton {
    CREEPER_DEFINE_CONSTROCTOR(TextButton, text_button::pro);
    using text_button::internal::TextButton::TextButton;
};
}
