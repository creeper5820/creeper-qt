#pragma once

#include "creeper-qt/utility/api/scope/common.hh"
#include "filled-button.hh"

namespace creeper {

class FilledTonalButton : public FilledButton {
public:
    explicit FilledTonalButton(auto&&... props)
        : FilledButton { } {
        construct_with(std::forward<decltype(props)>(props)...);
    }

    void loadColorScheme(const ColorScheme& color_scheme) {
        setBackground(color_scheme.secondary_container);
        setTextColor(color_scheme.on_secondary_container);

        auto water_color = QColor { };
        if (color_scheme.primary.lightness() > 128) {
            water_color = color_scheme.primary.darker(130);
            setHoverColor(QColor { 0, 0, 0, 30 });
        } else {
            water_color = color_scheme.primary.lighter(130);
            setHoverColor(QColor { 255, 255, 255, 30 });
        }
        water_color.setAlphaF(0.25);
        setWaterColor(water_color);

        update();
    }

    void bindThemeManager(ThemeManager& manager) {
        manager.appendHandler(
            this, [this](const ThemeManager& manager) { loadColorScheme(manager.colorScheme()); });
    }
};

namespace filled_tonal_button::pro {
    using namespace common::pro;
    using namespace api::scope::common;
    using namespace widget::pro;
    using namespace theme::pro;
    using namespace filled_button::pro;
}

}
