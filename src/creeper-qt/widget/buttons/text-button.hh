#pragma once

#include "creeper-qt/utility/api/pro/clickable.hh" // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/common.hh"  // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/theme.hh"   // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/widget.hh"  // IWYU pragma: keep

#include "filled-button.hh"

namespace creeper {

class TextButton : public FilledButton {
public:
    explicit TextButton(auto&&... props)
        : FilledButton { } {
        construct_with(std::forward<decltype(props)>(props)...);
    }

    void loadColorScheme(const ColorScheme& color_scheme) {
        setBackground(Qt::transparent);
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

    void bindThemeManager(ThemeManager& manager) {
        manager.appendHandler(
            this, [this](const ThemeManager& manager) { loadColorScheme(manager.colorScheme()); });
    }
};

namespace text_button::pro {
    using api::pro::Clickable;

    using namespace api::scope::common;
    using namespace filled_button::pro;
    using namespace api::scope::theme;
    using namespace api::scope::widget;
}

}
