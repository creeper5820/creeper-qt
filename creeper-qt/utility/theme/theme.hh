#pragma once

#include <qwidget.h>

#include "creeper-qt/utility/theme/color-scheme.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"
#include "creeper-qt/utility/wrapper/property.hh"

namespace creeper::theme {

struct ThemePack {
    ColorScheme light, dark;
    auto color_scheme(this auto&& self, ColorMode mode) noexcept {
        return (mode == ColorMode::LIGHT) ? self.light : self.dark;
    }
};

class ThemeManager {
    CREEPER_PIMPL_DEFINITION(ThemeManager)
public:
    explicit ThemeManager(const ThemePack& pack, ColorMode mode = ColorMode::LIGHT);

    void apply_theme() const;

    using Handler = std::function<void(const ThemeManager&)>;

    /// Registers a theme change callback for the specified widget.
    ///
    /// When ThemeManager::apply_theme() is called, the registered handler will be executed.
    ///
    /// Args:
    ///   key: Pointer to the widget. Serves as the key in the handler map.
    ///   handler: The callback function to register.
    ///
    /// Note:
    ///   When the widget is destroyed, ThemeManager::remove_handler() will be called automatically
    ///   to remove the associated handler.
    void append_handler(const QObject* key, const Handler& handler);

    void remove_handler(const QObject* key);

    void set_theme_pack(const ThemePack& pack);
    void set_color_mode(const ColorMode& mode);
    void toggle_color_mode();

    ThemePack theme_pack() const;
    ColorMode color_mode() const;

    ColorScheme color_scheme() const;
};

template <class T>
concept color_scheme_setter_trait = requires(T t) {
    { t.set_color_scheme(ColorScheme {}) };
};
template <class T>
concept theme_manager_loader_trait =
    requires(T t, ThemeManager& manager) { t.load_theme_manager(manager); };

}
namespace creeper::theme::pro {

using Token = common::Token<ThemeManager>;

struct ColorScheme : public theme::ColorScheme, Token {
    using theme::ColorScheme::ColorScheme;
    explicit ColorScheme(const theme::ColorScheme& p)
        : theme::ColorScheme(p) { }
    auto apply(color_scheme_setter_trait auto& self) const noexcept -> void {
        self.set_color_scheme(*this);
    }
};

struct ThemeManager : Token {
    theme::ThemeManager& manager;
    explicit ThemeManager(theme::ThemeManager& p)
        : manager(p) { }
    auto apply(theme_manager_loader_trait auto& self) const noexcept -> void {
        self.load_theme_manager(manager);
    }
};

template <typename T>
concept trait = std::derived_from<T, Token>;

CREEPER_DEFINE_CHECK(trait);

}
namespace creeper {

using ColorMode    = theme::ColorMode;
using ColorScheme  = theme::ColorScheme;
using ThemePack    = theme::ThemePack;
using ThemeManager = theme::ThemeManager;

}
