#pragma once

#include "creeper-qt/utility/theme/theme.hh"

namespace creeper {

// 橄榄绿亮色主题 (Light Theme)
constexpr auto kGreenLightColorScheme = ColorScheme {
    // Light Scheme (Primary: #386A20)
    .primary              = QColor(56, 106, 32),
    .on_primary           = QColor(255, 255, 255),
    .primary_container    = QColor(184, 246, 150),
    .on_primary_container = QColor(1, 34, 0),

    .secondary              = QColor(86, 98, 75),
    .on_secondary           = QColor(255, 255, 255),
    .secondary_container    = QColor(217, 231, 202),
    .on_secondary_container = QColor(20, 31, 11),

    .tertiary              = QColor(56, 102, 99),
    .on_tertiary           = QColor(255, 255, 255),
    .tertiary_container    = QColor(187, 236, 231),
    .on_tertiary_container = QColor(0, 32, 31),

    .error              = QColor(186, 26, 26),
    .on_error           = QColor(255, 255, 255),
    .error_container    = QColor(255, 218, 214),
    .on_error_container = QColor(65, 0, 2),

    .background         = QColor(252, 253, 246),
    .on_background      = QColor(26, 28, 24),
    .surface            = QColor(252, 253, 246),
    .on_surface         = QColor(26, 28, 24),
    .surface_variant    = QColor(222, 229, 212),
    .on_surface_variant = QColor(67, 72, 62),

    .outline         = QColor(116, 121, 109),
    .outline_variant = QColor(195, 201, 188),
    .shadow          = QColor(0, 0, 0),
    .scrim           = QColor(0, 0, 0),

    .inverse_surface    = QColor(47, 49, 45),
    .inverse_on_surface = QColor(241, 241, 235),
    .inverse_primary    = QColor(157, 218, 125),

    // Surface steps for Light Theme
    .surface_container_highest = QColor(230, 230, 223), // Surface 4
    .surface_container_high    = QColor(236, 236, 229), // Surface 3
    .surface_container         = QColor(241, 241, 235), // Surface 2
    .surface_container_low     = QColor(246, 247, 240), // Surface 1
    .surface_container_lowest  = QColor(255, 255, 255), // Surface 0
};

// 橄榄绿暗色主题 (Dark Theme)
constexpr auto kGreenDarkColorScheme = ColorScheme {
    // Dark Scheme (Primary: #9DDA7D)
    .primary              = QColor(157, 218, 125),
    .on_primary           = QColor(15, 57, 0),
    .primary_container    = QColor(33, 81, 6),
    .on_primary_container = QColor(184, 246, 150),

    .secondary              = QColor(189, 203, 176),
    .on_secondary           = QColor(42, 52, 32),
    .secondary_container    = QColor(64, 74, 54),
    .on_secondary_container = QColor(217, 231, 202),

    .tertiary              = QColor(160, 208, 204),
    .on_tertiary           = QColor(1, 55, 53),
    .tertiary_container    = QColor(31, 78, 76),
    .on_tertiary_container = QColor(187, 236, 231),

    .error              = QColor(255, 180, 171),
    .on_error           = QColor(105, 0, 5),
    .error_container    = QColor(147, 0, 10),
    .on_error_container = QColor(255, 218, 214),

    .background         = QColor(26, 28, 24),
    .on_background      = QColor(227, 227, 220),
    .surface            = QColor(26, 28, 24),
    .on_surface         = QColor(227, 227, 220),
    .surface_variant    = QColor(67, 72, 62),
    .on_surface_variant = QColor(195, 201, 188),

    .outline         = QColor(142, 146, 135),
    .outline_variant = QColor(67, 72, 62),
    .shadow          = QColor(0, 0, 0),
    .scrim           = QColor(0, 0, 0),

    .inverse_surface    = QColor(227, 227, 220),
    .inverse_on_surface = QColor(47, 49, 45),
    .inverse_primary    = QColor(56, 106, 32),

    // Surface steps for Dark Theme
    .surface_container_highest = QColor(60, 65, 60), // Surface 4
    .surface_container_high    = QColor(49, 54, 49), // Surface 3
    .surface_container         = QColor(38, 43, 37), // Surface 2
    .surface_container_low     = QColor(34, 37, 33), // Surface 1
    .surface_container_lowest  = QColor(15, 20, 12), // Surface 0
};

// 橄榄绿主题包
constexpr auto kGreenThemePack = ThemePack {
    .light = kGreenLightColorScheme,
    .dark  = kGreenDarkColorScheme,
};

}
