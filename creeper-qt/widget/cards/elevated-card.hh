#pragma once
#include "basic-card.hh"

namespace creeper {
namespace elevated_card::internal {
    class ElevatedCard : public card::internal::BasicCard {
    public:
        explicit ElevatedCard() {
            using namespace card::internal;
            shadow_effect.setBlurRadius(kElevatedShadowBlurRadius);
            shadow_effect.setOffset(kElevatedShadowOffsetX, kElevatedShadowOffsetY);
            setGraphicsEffect(&shadow_effect);
        }

        void set_color_scheme(const ColorScheme& scheme) {
            using namespace card::internal;

            auto shadow_color = scheme.shadow;
            shadow_color.setAlphaF(kElevatedShadowOpacity);

            shadow_effect.setColor(shadow_color);
            BasicCard::set_color_scheme(scheme);
        }

        void load_theme_manager(ThemeManager& manager) {
            manager.append_handler(this,
                [this](const ThemeManager& manager) { set_color_scheme(manager.color_scheme()); });
        }

    private:
        QGraphicsDropShadowEffect shadow_effect {};
    };

}
namespace elevated_card::pro {
    template <class Card>
    concept property_concept = card::pro::property_concept<Card>;
    using namespace card::pro;
}
class ElevatedCard : public elevated_card::internal::ElevatedCard {
    CREEPER_DEFINE_CONSTROCTOR(ElevatedCard, elevated_card::pro);
    using elevated_card::internal::ElevatedCard::ElevatedCard;
};
}
