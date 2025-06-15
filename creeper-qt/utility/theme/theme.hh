#pragma once

#include <qwidget.h>

#include "utility/theme/color-scheme.hh"
#include "utility/wrapper/pimpl.hh"

namespace creeper::util::theme {

struct ThemePack {
    ColorScheme light;
    ColorScheme dark;

    const ColorScheme& color_scheme(ColorMode mode) const noexcept {
        return mode == ColorMode::LIGHT ? light : dark;
    }
    ColorScheme& color_scheme(ColorMode mode) noexcept {
        return mode == ColorMode::LIGHT ? light : dark;
    }
};

class ThemeManager {
    CREEPER_PIMPL_DEFINTION(ThemeManager)
public:
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
    void append_handler(const QWidget* key, const Handler& handler);
    void remove_handler(const QWidget* key);

    void set_theme_pack(const ThemePack& pack);
    void set_color_mode(const ColorMode& mode);

    ThemePack theme_pack() const;
    ColorMode color_mode() const;

    ColorScheme color_scheme() const;
};

namespace pro {

    template <class Widget, class Token>
        requires requires(Widget widget) { widget.set_color_scheme(ColorScheme {}); }
    struct ColorScheme final : public util::theme::ColorScheme, Token {
        using util::theme::ColorScheme::ColorScheme;
        explicit ColorScheme(const util::theme::ColorScheme& p)
            : util::theme::ColorScheme(p) { }
        void apply(Widget& self) const override { self.set_color_scheme(*this); }
    };

    template <class Widget, class Token>
        requires requires(Widget widget, util::theme::ThemeManager& manager) {
            widget.load_theme_manager(manager);
        }
    struct ThemeManager final : Token {
        util::theme::ThemeManager& manager;
        explicit ThemeManager(util::theme::ThemeManager& p)
            : manager(p) { }
        void apply(Widget& self) const override { self.load_theme_manager(manager); }
    };

}

}

namespace creeper {

using ColorMode    = util::theme::ColorMode;
using ColorScheme  = util::theme::ColorScheme;
using ThemePack    = util::theme::ThemePack;
using ThemeManager = util::theme::ThemeManager;

constexpr auto kBlueMikuThemePack = ThemePack {
    .light = util::theme::kBlueMikuLightColorScheme,
    .dark  = util::theme::kBlueMikuDarkColorScheme,
};

}
