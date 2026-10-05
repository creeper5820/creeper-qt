#pragma once

#include "creeper-qt/utility/theme/theme.hh"

namespace creeper::api::pro {

/**
 * @brief 绑定主题管理器
 *
 * 要求组件实现 void bindThemeManager(ThemeManager&)。
 *
 * @note ThemeManager 可隐式转换为 BindTheme，因此传入裸 ThemeManager& 亦可作为属性。
 */
struct BindTheme {
    ThemeManager& manager;

    explicit BindTheme(ThemeManager& p) noexcept
        : manager(p) { }

    friend auto dsl_invoke(auto& self, const BindTheme& prop) -> void
        requires requires(ThemeManager& manager) { self.bindThemeManager(manager); }
    {
        self.bindThemeManager(prop.manager);
    }
};

}
namespace creeper {

auto dsl_invoke(auto& self, ThemeManager& manager) -> void
    requires requires { self.bindThemeManager(manager); }
{
    self.bindThemeManager(manager);
}

}
