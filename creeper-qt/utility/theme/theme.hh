#pragma once

#include <qwidget.h>

#include "creeper-qt/utility/theme/color-scheme.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/pimpl.hh"

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
    void append_handler(const QWidget* key, const Handler& handler);

    void remove_handler(const QWidget* key);

    void set_theme_pack(const ThemePack& pack);
    void set_color_mode(const ColorMode& mode);
    void toggle_color_mode();

    ThemePack theme_pack() const;
    ColorMode color_mode() const;

    ColorScheme color_scheme() const;
};

namespace pro {

    using Token = common::Token<ThemeManager>;

    template <typename T>
    concept concept_ = std::derived_from<T, Token>;

    struct ColorScheme : public util::theme::ColorScheme, Token {
        using util::theme::ColorScheme::ColorScheme;
        explicit ColorScheme(const util::theme::ColorScheme& p)
            : util::theme::ColorScheme(p) { }
        void apply(auto& self) const
            requires requires { self.set_color_scheme(*this); }
        {
            self.set_color_scheme(*this);
        }
    };

    struct ThemeManager : Token {
        util::theme::ThemeManager& manager;
        explicit ThemeManager(util::theme::ThemeManager& p)
            : manager(p) { }
        void apply(auto& self) const
            requires requires { self.load_theme_manager(manager); }
        {
            self.load_theme_manager(manager);
        }
    };

}

}

namespace creeper {

using ColorMode    = util::theme::ColorMode;
using ColorScheme  = util::theme::ColorScheme;
using ThemePack    = util::theme::ThemePack;
using ThemeManager = util::theme::ThemeManager;

}
