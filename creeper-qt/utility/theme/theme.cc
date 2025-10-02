#include "creeper-qt/utility/theme/theme.hh"

using namespace creeper::theme;
using Handler = ThemeManager::Handler;

struct ThemeManager::Impl {
    using Key = const QObject*;

    std::unordered_map<Key, Handler> handlers;
    ThemePack theme_pack;
    ColorMode color_mode;

    auto apply_theme(const ThemeManager& manager) const {
        for (auto& [_, handler] : handlers)
            handler(manager);
    }

    auto append_handler(Key key, const Handler& handler) {
        handlers[key] = handler;
        QObject::connect(key, &QObject::destroyed, [this, key] { remove_handler(key); });
    }

    void remove_handler(Key key) { handlers.erase(key); }
};

ThemeManager::ThemeManager()
    : pimpl(std::make_unique<Impl>()) { }

ThemeManager::ThemeManager(const ThemePack& pack, ColorMode mode)
    : pimpl(std::make_unique<Impl>()) {
    pimpl->theme_pack = pack;
    pimpl->color_mode = mode;
}

ThemeManager::~ThemeManager() = default;

void ThemeManager::apply_theme() const { pimpl->apply_theme(*this); }

void ThemeManager::append_handler(const QObject* key, const Handler& handler) {
    pimpl->append_handler(key, handler);
}

void ThemeManager::remove_handler(const QObject* key) { pimpl->remove_handler(key); }

void ThemeManager::set_theme_pack(const ThemePack& pack) { pimpl->theme_pack = pack; }
void ThemeManager::set_color_mode(const ColorMode& mode) { pimpl->color_mode = mode; }

void ThemeManager::toggle_color_mode() {
    pimpl->color_mode = (pimpl->color_mode == ColorMode::LIGHT) //
        ? ColorMode::DARK
        : ColorMode::LIGHT;
}

ThemePack ThemeManager::theme_pack() const { return pimpl->theme_pack; }
ColorMode ThemeManager::color_mode() const { return pimpl->color_mode; }

ColorScheme ThemeManager::color_scheme() const {
    return pimpl->theme_pack.color_scheme(pimpl->color_mode);
}
