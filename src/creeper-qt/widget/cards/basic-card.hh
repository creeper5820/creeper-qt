#pragma once

#include "creeper-qt/utility/api/scope/theme.hh"

#include "creeper-qt/utility/api/scope/widget.hh"

#include "creeper-qt/utility/api/scope/common.hh"
#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/utility/wrapper/forward_prop.hh"
#include "creeper-qt/widget/shape/rounded-rect.hh"

namespace creeper::card {

constexpr auto kCardRadius = double { 12 };

constexpr auto kElevatedShadowOpacity    = double { 0.4 };
constexpr auto kElevatedShadowBlurRadius = double { 10 };
constexpr auto kElevatedShadowOffsetX    = double { 0 };
constexpr auto kElevatedShadowOffsetY    = double { 2 };

constexpr auto kOutlinedWidth = double { 1.5 };

}

namespace creeper {

class Card : public RoundedRect {
public:
    enum class Level {
        LOWEST,
        LOW,
        DEFAULT,
        HIGH,
        HIGHEST,
    };

    using RoundedRect::RoundedRect;

    explicit Card() {
        construct_with(rounded_rect::pro::BorderWidth { 0 },
            rounded_rect::pro::BorderColor { Qt::transparent },
            rounded_rect::pro::Radius { card::kCardRadius });
    }

    explicit Card(auto&&... props)
        : Card { } {
        construct_with(std::forward<decltype(props)>(props)...);
    }

    Level level = Level::DEFAULT;

    auto setLevel(Level level) noexcept -> void {
        this->level = level;
        update();
    }

    auto loadColorScheme(const ColorScheme& scheme) -> void {
        switch (level) {
        case Level::LOWEST:
            setBackground(scheme.surface_container_lowest);
            break;
        case Level::LOW:
            setBackground(scheme.surface_container_low);
            break;
        case Level::DEFAULT:
            setBackground(scheme.surface_container);
            break;
        case Level::HIGH:
            setBackground(scheme.surface_container_high);
            break;
        case Level::HIGHEST:
            setBackground(scheme.surface_container_highest);
            break;
        }
        update();
    }

    auto bindThemeManager(ThemeManager& manager) -> void {
        manager.appendHandler(
            this, [this](const ThemeManager& manager) { loadColorScheme(manager.colorScheme()); });
    }
};

namespace card::pro {
    using Level                 = ForwardProp<&Card::setLevel>;
    constexpr auto LevelDefault = Level { Card::Level::DEFAULT };
    constexpr auto LevelHigh    = Level { Card::Level::HIGH };
    constexpr auto LevelHighest = Level { Card::Level::HIGHEST };
    constexpr auto LevelLow     = Level { Card::Level::LOW };
    constexpr auto LevelLowest  = Level { Card::Level::LOWEST };

    using namespace api::scope::common;
    using namespace rounded_rect::pro;
    using namespace api::scope::theme;
    using namespace api::scope::widget;
}

using CardLevel = Card::Level;

using BasicCard = Card;

}
