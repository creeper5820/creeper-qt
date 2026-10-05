#pragma once

#include "creeper-qt/utility/api/scope/common.hh"
#include "filled-button.hh"

namespace creeper {

class OutlinedButton : public FilledButton {
public:
    explicit OutlinedButton(auto&&... props)
        : FilledButton { } {
        construct_with(std::forward<decltype(props)>(props)...);
    }

    void loadColorScheme(const ColorScheme& color_scheme) {
        setBackground(Qt::transparent);
        setBorderColor(color_scheme.outline);
        setTextColor(color_scheme.primary);

        auto hover_color = color_scheme.primary;
        hover_color.setAlphaF(0.08);
        setHoverColor(hover_color);

        auto water_color = QColor { };
        if (color_scheme.primary.lightness() > 128) {
            water_color = color_scheme.primary.darker(130);
        } else {
            water_color = color_scheme.primary.lighter(130);
        }
        water_color.setAlphaF(0.25);
        setWaterColor(water_color);

        update();
    }

    auto bindThemeManager(ThemeManager& manager) noexcept -> void {
        setBorderWidth(1.5);
        manager.appendHandler(
            this, [this](const ThemeManager& manager) { loadColorScheme(manager.colorScheme()); });
    }
};

namespace outlined_button::pro {
    using namespace common::pro;
    using namespace api::scope::common;
    using namespace widget::pro;
    using namespace theme::pro;
    using namespace filled_button::pro;
}

}
