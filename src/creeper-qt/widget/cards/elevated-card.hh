#pragma once
#include "basic-card.hh"

namespace creeper {

class ElevatedCard : public Card {
public:
    explicit ElevatedCard(auto&&... props)
        : Card { } {
        shadow_effect.setBlurRadius(card::kElevatedShadowBlurRadius);
        shadow_effect.setOffset(card::kElevatedShadowOffsetX, card::kElevatedShadowOffsetY);
        setGraphicsEffect(&shadow_effect);

        construct_with(std::forward<decltype(props)>(props)...);
    }

    auto loadColorScheme(const ColorScheme& scheme) -> void {
        auto shadow_color = scheme.shadow;
        shadow_color.setAlphaF(card::kElevatedShadowOpacity);

        shadow_effect.setColor(shadow_color);
        Card::loadColorScheme(scheme);
    }

    auto bindThemeManager(ThemeManager& manager) -> void {
        manager.appendHandler(
            this, [this](const ThemeManager& manager) { loadColorScheme(manager.colorScheme()); });
    }

private:
    QGraphicsDropShadowEffect shadow_effect { };
};

namespace elevated_card::pro {
    using namespace card::pro;
}

}
