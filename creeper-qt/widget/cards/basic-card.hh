#pragma once
#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/widget/shape/rounded-rect.hh"

namespace creeper::card::internal {

constexpr auto kCardRadius = double { 12 };

constexpr auto kElevatedShadowOpacity    = double { 0.4 };
constexpr auto kElevatedShadowBlurRadius = double { 10 };
constexpr auto kElevatedShadowOffsetX    = double { 0 };
constexpr auto kElevatedShadowOffsetY    = double { 2 };

constexpr auto kOutlinedWidth = double { 1.5 };

class BasicCard : public RoundedRect {
public:
    enum class Level { LOWEST, LOW, DEFAULT, HIGH, HIGHEST } level { Level::DEFAULT };

public:
    explicit BasicCard()
        : RoundedRect {
            rounded_rect::pro::BorderWidth { 0 },
            rounded_rect::pro::BorderColor { Qt::transparent },
            rounded_rect::pro::Radius { kCardRadius },
        } { }

    void set_color_scheme(const ColorScheme& scheme) {
        switch (level) {
        case card::internal::BasicCard::Level::LOWEST:
            set_background(scheme.surface_container_lowest);
            break;
        case card::internal::BasicCard::Level::LOW:
            set_background(scheme.surface_container_low);
            break;
        case card::internal::BasicCard::Level::DEFAULT:
            set_background(scheme.surface_container);
            break;
        case card::internal::BasicCard::Level::HIGH:
            set_background(scheme.surface_container_high);
            break;
        case card::internal::BasicCard::Level::HIGHEST:
            set_background(scheme.surface_container_highest);
            break;
        }
        update();
    }

    void load_theme_manager(ThemeManager& manager) {
        manager.append_handler(this,
            [this](const ThemeManager& manager) { set_color_scheme(manager.color_scheme()); });
    }
};
}
namespace creeper::card::pro {

using Token = common::Token<internal::BasicCard>;

struct Level : Token {
    using Enum = internal::BasicCard::Level;
    Enum level { Enum::DEFAULT };

    explicit Level(Enum level)
        : level(level) { }

    void apply(auto& self) const { self.level = level; }
};

template <class Card>
concept property_concept = std::derived_from<Card, Token> //
    || rounded_rect::pro::property_concept<Card>          //
    || util::theme::pro::property_concept<Card>;

using namespace rounded_rect::pro;
using namespace util::theme::pro;
}
namespace creeper {
using SurfaceLevel = card::internal::BasicCard::Level;
}
