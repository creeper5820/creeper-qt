#include "creeper-qt/utility/theme/theme.hh"

using namespace creeper;
using Handler = ThemeManager::Handler;

struct ThemeManager::Impl {
    using Key = const QObject*;

    std::unordered_map<Key, Handler> handlers;
    std::vector<Handler> begin_callbacks;
    std::vector<Handler> final_callbacks;
    ThemePack theme_pack;
    ColorMode color_mode;

    auto applyTheme(const ThemeManager& manager) const {
        for (auto const& callback : begin_callbacks)
            callback(manager);
        for (auto& [_, callback] : handlers)
            callback(manager);
        for (auto const& callback : final_callbacks)
            callback(manager);
    }

    auto appendHandler(Key key, const Handler& handler) {
        handlers[key] = handler;
        QObject::connect(key, &QObject::destroyed, [this, key] { removeHandler(key); });
    }

    void removeHandler(Key key) { handlers.erase(key); }
};

ThemeManager::ThemeManager()
    : pimpl(std::make_unique<Impl>()) { }

ThemeManager::ThemeManager(const ThemePack& pack, ColorMode mode)
    : pimpl(std::make_unique<Impl>()) {
    pimpl->theme_pack = pack;
    pimpl->color_mode = mode;
}

ThemeManager::~ThemeManager() = default;

void ThemeManager::applyTheme() const { pimpl->applyTheme(*this); }

void ThemeManager::appendHandler(const QObject* key, const Handler& handler) {
    pimpl->appendHandler(key, handler);
}

auto ThemeManager::appendBeginCallback(const Handler& callback) noexcept -> void {
    pimpl->begin_callbacks.push_back(callback);
}
auto ThemeManager::appendFinalCallback(const Handler& callback) noexcept -> void {
    pimpl->final_callbacks.push_back(callback);
}

void ThemeManager::removeHandler(const QObject* key) { pimpl->removeHandler(key); }

void ThemeManager::setThemePack(const ThemePack& pack) { pimpl->theme_pack = pack; }
void ThemeManager::setColorMode(const ColorMode& mode) { pimpl->color_mode = mode; }

void ThemeManager::toggleColorMode() {
    pimpl->color_mode = (pimpl->color_mode == ColorMode::LIGHT) //
        ? ColorMode::DARK
        : ColorMode::LIGHT;
}

ThemePack ThemeManager::themePack() const { return pimpl->theme_pack; }
ColorMode ThemeManager::colorMode() const { return pimpl->color_mode; }

ColorScheme ThemeManager::colorScheme() const {
    return pimpl->theme_pack.colorScheme(pimpl->color_mode);
}
