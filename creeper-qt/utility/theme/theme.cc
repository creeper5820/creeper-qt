#include "utility/theme/theme.hh"

using namespace creeper::util::theme;
using Handler = ThemeManager::Handler;

struct ThemeManager::Impl {
    std::unordered_map<const QWidget*, Handler> handlers;
    ThemePack theme_pack;
    ColorMode color_mode;

    void apply_theme(const ThemeManager& manager) const {
        for (auto& [_, handler] : handlers)
            handler(manager);
    }

    void append_handler(const QWidget* key, const Handler& handler) {
        handlers[key] = handler;
        QObject::connect(key, &QWidget::destroyed, [this, key] { remove_handler(key); });
    }

    void remove_handler(const QWidget* key) { handlers.erase(key); }

    void set_theme_pack(const ThemePack& pack) { theme_pack = pack; }
    void set_color_mode(const ColorMode& mode) { color_mode = mode; }
};

ThemeManager::ThemeManager()
    : pimpl(std::make_unique<Impl>()) { }

ThemeManager::~ThemeManager() = default;

void ThemeManager::apply_theme() const { pimpl->apply_theme(*this); }

void ThemeManager::append_handler(const QWidget* key, const Handler& handler) {
    pimpl->append_handler(key, handler);
}

void ThemeManager::remove_handler(const QWidget* key) { pimpl->remove_handler(key); }

void ThemeManager::set_theme_pack(const ThemePack& pack) { pimpl->set_theme_pack(pack); }
void ThemeManager::set_color_mode(const ColorMode& mode) { pimpl->set_color_mode(mode); }

ThemePack ThemeManager::theme_pack() const { return pimpl->theme_pack; }
ColorMode ThemeManager::color_mode() const { return pimpl->color_mode; }

ColorScheme ThemeManager::color_scheme() const {
    return pimpl->theme_pack.color_scheme(pimpl->color_mode);
}
