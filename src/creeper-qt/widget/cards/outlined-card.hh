#pragma once
#include "basic-card.hh"

namespace creeper::outlined_card::internal {

class OutlinedCard : public BasicCard {
public:
    explicit OutlinedCard() {
        using namespace card::internal;
        set_border_width(kOutlinedWidth);
    }

    void set_color_scheme(const ColorScheme& scheme) {
        set_border_color(scheme.outline_variant);
        Card::set_color_scheme(scheme);
    }

    void load_theme_manager(ThemeManager& manager) {
        manager.append_handler(this,
            [this](const ThemeManager& manager) { set_color_scheme(manager.color_scheme()); });
    }
};

}
namespace creeper::outlined_card::pro {
using namespace card::pro;
}
namespace creeper {

using OutlinedCard = Declarative<outlined_card::internal::OutlinedCard,
    CheckerOr<card::pro::checker, rounded_rect::pro::checker, theme::pro::checker,
        widget::pro::checker>>;

}
