#pragma once
#include "basic-card.hh"

namespace creeper {
namespace outlined_card::internal {
    class OutlinedCard : public card::internal::BasicCard {
    public:
        explicit OutlinedCard() {
            using namespace card::internal;
            set_border_width(kOutlinedWidth);
        }

        void set_color_scheme(const ColorScheme& scheme) {
            set_border_color(scheme.outline_variant);
            BasicCard::set_color_scheme(scheme);
        }

        void load_theme_manager(ThemeManager& manager) {
            manager.append_handler(this,
                [this](const ThemeManager& manager) { set_color_scheme(manager.color_scheme()); });
        }
    };

}
namespace outlined_card::pro {
    template <class Card>
    concept property_concept = card::pro::property_concept<Card>;
}
class OutlinedCard : public outlined_card::internal::OutlinedCard {
    CREEPER_DEFINE_CONSTROCTOR(OutlinedCard, outlined_card::pro);
    using outlined_card::internal::OutlinedCard::OutlinedCard;
};
}
