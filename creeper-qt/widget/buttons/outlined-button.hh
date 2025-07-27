#pragma once
#include "filled-button.hh"

namespace creeper {
namespace outlined_button::internal {
    class OutlinedButton : public filled_button::internal::FilledButton {
    public:
        explicit OutlinedButton()
            : FilledButton {} {
            set_border_width(1.5);
        }

        void set_color_scheme(const ColorScheme& color_scheme) {
            set_background(Qt::transparent);
            set_border_color(color_scheme.outline);
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

namespace outlined_button::pro {

    template <typename T>
    concept property_concept =
        util::theme::pro::property_concept<T> || button::pro::property_concept<T>;

    struct checker {
        template <class T> struct result {
            static constexpr auto v = false;
        };
        template <property_concept T> struct result<T> {
            static constexpr auto v = true;
        };
    };

    using namespace util::theme::pro;
    using namespace button::pro;
}
using OutlinedButton =
    Declarative<outlined_button::internal::OutlinedButton, outlined_button::pro::checker>;
}
