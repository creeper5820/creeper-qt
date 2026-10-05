#pragma once

#include "creeper-qt/utility/theme/theme.hh"

namespace creeper::api::pro {

/**
 * @brief 绑定主题管理器
 *
 * 要求组件实现 void bindThemeManager(theme::ThemeManager&)。
 */
struct ThemeManager {
    theme::ThemeManager& manager;

    explicit ThemeManager(theme::ThemeManager& p)
        : manager(p) { }

    friend auto dsl_invoke(auto& self, const ThemeManager& prop) -> void
        requires requires(theme::ThemeManager& manager) { self.bindThemeManager(manager); }
    {
        self.bindThemeManager(prop.manager);
    }
};

}
