#pragma once

#include <qcolor.h>
#include <qfont.h>

#include <concepts>

namespace creeper {

enum class ColorMode { LIGHT, DARK };

struct ColorScheme {
    QColor primary;
    QColor on_primary;
    QColor primary_container;
    QColor on_primary_container;

    QColor secondary;
    QColor on_secondary;
    QColor secondary_container;
    QColor on_secondary_container;

    QColor tertiary;
    QColor on_tertiary;
    QColor tertiary_container;
    QColor on_tertiary_container;

    QColor error;
    QColor on_error;
    QColor error_container;
    QColor on_error_container;

    QColor background;
    QColor on_background;
    QColor surface;
    QColor on_surface;
    QColor surface_variant;
    QColor on_surface_variant;

    QColor outline;
    QColor outline_variant;
    QColor shadow;
    QColor scrim;

    QColor inverse_surface;
    QColor inverse_on_surface;
    QColor inverse_primary;

    QColor surface_container_highest;
    QColor surface_container_high;
    QColor surface_container;
    QColor surface_container_low;
    QColor surface_container_lowest;

    /// 声明式属性：要求目标组件实现 void loadColorScheme(const ColorScheme&)
    friend auto dsl_invoke(auto& self, const ColorScheme& scheme) -> void
        requires requires { self.loadColorScheme(scheme); }
    {
        self.loadColorScheme(scheme);
    }
};

/// 约束组件可接收配色方案：要求实现 void loadColorScheme(const ColorScheme&)
template <class T>
concept color_scheme_setter_trait = requires(T t) {
    { t.loadColorScheme(ColorScheme { }) };
};

struct Typography {
    QFont body;
    QFont title;
    QFont button;
};

}
