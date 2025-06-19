#include "widget/button/filled-button.hh"

namespace creeper {
namespace filled_tonal_button::internal {
    class FilledTonalButton : public FilledButton {
    public:
        void set_color_scheme(const ColorScheme& color_scheme) {
            set_background(color_scheme.secondary_container);
            set_text_color(color_scheme.on_secondary_container);

            auto water_color = QColor {};
            if (color_scheme.primary.lightness() > 128) {
                water_color = color_scheme.primary.darker(130);
                set_hover_color(QColor { 0, 0, 0, 30 });
            } else {
                water_color = color_scheme.primary.lighter(130);
                set_hover_color(QColor { 255, 255, 255, 30 });
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

namespace filled_tonal_button::pro {

    template <typename T>
    concept property_concept =
        util::theme::pro::property_concept<T> || button::pro::property_concept<T>;

    using namespace util::theme::pro;
    using namespace button::pro;
}

class FilledTonalButton : public filled_tonal_button::internal::FilledTonalButton {
    CREEPER_DEFINE_CONSTROCTOR(FilledTonalButton, filled_tonal_button::pro);
    using filled_tonal_button::internal::FilledTonalButton::FilledTonalButton;
};

}
