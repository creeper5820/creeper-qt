#pragma once

#include "creeper-qt/utility/theme/color-scheme.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"

#include <qwidget.h>

namespace creeper {

struct ThemePack {
    ColorScheme light, dark;
    auto colorScheme(this auto&& self, ColorMode mode) noexcept {
        return (mode == ColorMode::LIGHT) ? self.light : self.dark;
    }
};

class ThemeManager {
    CREEPER_PIMPL_DEFINITION(ThemeManager)
public:
    explicit ThemeManager(const ThemePack& pack, ColorMode mode = ColorMode::LIGHT);

    void applyTheme() const;

    using Handler = std::function<void(const ThemeManager&)>;

    /// Registers a theme change callback for the specified widget.
    ///
    /// When ThemeManager::applyTheme() is called, the registered handler will be executed.
    ///
    /// Args:
    ///   key: Pointer to the widget. Serves as the key in the handler map.
    ///   handler: The callback function to register.
    ///
    /// Note:
    ///   When the widget is destroyed, ThemeManager::removeHandler() will be called automatically
    ///   to remove the associated handler.
    void appendHandler(const QObject* key, const Handler& handler);

    auto appendHandler(color_scheme_setter_trait auto& widget) { appendHandler(&widget, widget); }
    auto appendHandler(const QObject* key, color_scheme_setter_trait auto& widget) {
        const auto handler = [&widget](const ThemeManager& manager) {
            const auto color_mode = manager.colorMode();
            const auto theme_pack = manager.themePack();
            widget.loadColorScheme(theme_pack.colorScheme(color_mode));
        };
        appendHandler(key, std::move(handler));
    }

    auto appendBeginCallback(const Handler&) noexcept -> void;
    auto appendFinalCallback(const Handler&) noexcept -> void;

    void removeHandler(const QObject* key);

    void setThemePack(const ThemePack& pack);
    void setColorMode(const ColorMode& mode);
    void toggleColorMode();

    ThemePack themePack() const;
    ColorMode colorMode() const;

    ColorScheme colorScheme() const;
};

}
