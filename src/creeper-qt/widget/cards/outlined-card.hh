#pragma once
#include "basic-card.hh"

namespace creeper {

class OutlinedCard : public Card {
public:
    explicit OutlinedCard(auto&&... props)
        : Card { } {
        setBorderWidth(card::kOutlinedWidth);
        construct_with(std::forward<decltype(props)>(props)...);
    }

    auto loadColorScheme(const ColorScheme& scheme) -> void {
        setBorderColor(scheme.outline_variant);
        Card::loadColorScheme(scheme);
    }

    auto bindThemeManager(ThemeManager& manager) -> void {
        manager.appendHandler(
            this, [this](const ThemeManager& manager) { loadColorScheme(manager.colorScheme()); });
    }
};

namespace outlined_card::pro {
    using namespace card::pro;
}

}
