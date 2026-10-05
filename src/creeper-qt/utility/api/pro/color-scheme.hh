#pragma once

#include "creeper-qt/utility/theme/theme.hh"

namespace creeper::api::pro {

/**
 * @brief 载入配色方案
 *
 * 要求组件实现 void loadColorScheme(const theme::ColorScheme&)，
 * 由 color_scheme_setter_trait 约束。
 */
struct ColorScheme : public theme::ColorScheme {
    using theme::ColorScheme::ColorScheme;
    explicit ColorScheme(const theme::ColorScheme& p)
        : theme::ColorScheme(p) { }

    friend auto dsl_invoke(theme::color_scheme_setter_trait auto& self, const ColorScheme& prop)
        -> void {
        self.loadColorScheme(prop);
    }
};

}
