#pragma once
#include "basic-card.hh"

namespace creeper {
namespace outlined_card::internal {
    class Impl : public card::internal::Card {
    public:
        explicit Impl() {
            using namespace card::internal;
            set_border_width(kOutlinedWidth);
        }

        void set_color_scheme(const ColorScheme& scheme) {
            set_border_color(scheme.outline_variant);
            card::internal::Card::set_color_scheme(scheme);
        }

        void load_theme_manager(ThemeManager& manager) {
            manager.append_handler(this,
                [this](const ThemeManager& manager) { set_color_scheme(manager.color_scheme()); });
        }
    };

}
namespace outlined_card::pro {
    using namespace card::pro;
}
using OutlinedCard = Declarative<outlined_card::internal::Impl, card::pro::checker>;
}
