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

class Card : public RoundedRect {
public:
    enum class Level {
        LOWEST,
        LOW,
        DEFAULT,
        HIGH,
        HIGHEST,
    };

    Level level = Level::DEFAULT;

public:
    explicit Card() noexcept
        : Declarative {
            rounded_rect::pro::BorderWidth { 0 },
            rounded_rect::pro::BorderColor { Qt::transparent },
            rounded_rect::pro::Radius { kCardRadius },
        } { }

    void set_color_scheme(const ColorScheme& scheme) {
        switch (level) {
        case card::internal::Card::Level::LOWEST:
            set_background(scheme.surface_container_lowest);
            break;
        case card::internal::Card::Level::LOW:
            set_background(scheme.surface_container_low);
            break;
        case card::internal::Card::Level::DEFAULT:
            set_background(scheme.surface_container);
            break;
        case card::internal::Card::Level::HIGH:
            set_background(scheme.surface_container_high);
            break;
        case card::internal::Card::Level::HIGHEST:
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

using Token = common::Token<internal::Card>;

struct Level : Token {
    using Enum = internal::Card::Level;
    Enum level { Enum::DEFAULT };

    explicit Level(Enum level)
        : level { level } { }

    void apply(auto& self) const { self.level = level; }
};
namespace level {
    inline const auto DEFAULT = Level { Level::Enum::DEFAULT };
    inline const auto HIGHEST = Level { Level::Enum::HIGHEST };
    inline const auto HIGH    = Level { Level::Enum::HIGH };
    inline const auto LOWEST  = Level { Level::Enum::LOWEST };
    inline const auto LOW     = Level { Level::Enum::LOW };
}

template <class Card>
concept trait = std::derived_from<Card, Token>;

CREEPER_DEFINE_CHECKER(trait);

using namespace rounded_rect::pro;
using namespace theme::pro;
}
namespace creeper {

using CardLevel = card::internal::Card::Level;

using BasicCard = Declarative<card::internal::Card,
    CheckerOr<card::pro::checker, rounded_rect::pro::checker, theme::pro::checker,
        widget::pro::checker>>;

}
