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
            manager.append_handler(this, [this](const ThemeManager& manager) {
                const auto color_mode   = manager.color_mode();
                const auto theme_pack   = manager.theme_pack();
                const auto color_scheme = color_mode == ColorMode::LIGHT //
                    ? theme_pack.light
                    : theme_pack.dark;
                set_color_scheme(color_scheme);
            });
        }
    };
}

namespace filled_tonal_button::pro {
    using Property = common::InternalProperty<internal::FilledTonalButton>;

    template <typename T>
    concept property_concept = std::derived_from<T, Property> || widget::pro::property_concept<T>;

    using ColorScheme  = util::theme::pro::ColorScheme<internal::FilledTonalButton, Property>;
    using ThemeManager = util::theme::pro::ThemeManager<internal::FilledTonalButton, Property>;

    using Text        = common::pro::Text<internal::FilledTonalButton, Property>;
    using TextColor   = common::pro::TextColor<internal::FilledTonalButton, Property>;
    using Radius      = common::pro::Radius<internal::FilledTonalButton, Property>;
    using BorderWidth = common::pro::BorderWidth<internal::FilledTonalButton, Property>;
    using BorderColor = common::pro::BorderColor<internal::FilledTonalButton, Property>;
    using Background  = common::pro::Background<internal::FilledTonalButton, Property>;
    using WaterColor  = common::pro::WaterColor<internal::FilledTonalButton, Property>;
    using Clickable   = button::pro::Clickable<internal::FilledTonalButton, Property>;

    using namespace widget::pro;
}

class FilledTonalButton : public filled_tonal_button::internal::FilledTonalButton {
    CREEPER_DEFINE_CONSTROCTOR(FilledTonalButton, filled_tonal_button::pro);
    using filled_tonal_button::internal::FilledTonalButton::FilledTonalButton;
};

}
