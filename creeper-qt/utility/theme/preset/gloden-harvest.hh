#pragma once

#include "creeper-qt/utility/theme/theme.hh"

namespace creeper {

// 丰收金色亮色主题 (Golden Harvest Light Scheme)
constexpr auto kGoldenHarvestLightColorScheme = ColorScheme {
    .primary              = QColor(124, 94, 0),
    .on_primary           = QColor(255, 255, 255),
    .primary_container    = QColor(255, 223, 128),
    .on_primary_container = QColor(38, 28, 0),

    .secondary              = QColor(98, 91, 66),
    .on_secondary           = QColor(255, 255, 255),
    .secondary_container    = QColor(232, 224, 198),
    .on_secondary_container = QColor(30, 26, 10),

    .tertiary              = QColor(0, 77, 78),
    .on_tertiary           = QColor(255, 255, 255),
    .tertiary_container    = QColor(160, 232, 212),
    .on_tertiary_container = QColor(0, 32, 33),

    .error              = QColor(186, 26, 26),
    .on_error           = QColor(255, 255, 255),
    .error_container    = QColor(255, 218, 214),
    .on_error_container = QColor(65, 0, 2),

    .background         = QColor(255, 248, 236),
    .on_background      = QColor(30, 26, 10),
    .surface            = QColor(255, 248, 236),
    .on_surface         = QColor(30, 26, 10),
    .surface_variant    = QColor(232, 224, 198),
    .on_surface_variant = QColor(74, 69, 50),

    .outline         = QColor(124, 117, 89),
    .outline_variant = QColor(202, 196, 168),
    .shadow          = QColor(0, 0, 0),
    .scrim           = QColor(0, 0, 0),

    .inverse_surface    = QColor(51, 47, 30),
    .inverse_on_surface = QColor(250, 245, 230),
    .inverse_primary    = QColor(255, 223, 128),

    .surface_container_highest = QColor(240, 235, 220),
    .surface_container_high    = QColor(245, 240, 225),
    .surface_container         = QColor(250, 245, 230),
    .surface_container_low     = QColor(255, 250, 235),
    .surface_container_lowest  = QColor(255, 253, 240),
};

// 丰收金色暗色主题 (Golden Harvest Dark Scheme)
constexpr auto kGoldenHarvestDarkColorScheme = ColorScheme {
    .primary              = QColor(255, 223, 128),
    .on_primary           = QColor(38, 28, 0),
    .primary_container    = QColor(124, 94, 0),
    .on_primary_container = QColor(255, 255, 255),

    .secondary              = QColor(204, 196, 168),
    .on_secondary           = QColor(51, 47, 30),
    .secondary_container    = QColor(74, 69, 50),
    .on_secondary_container = QColor(232, 224, 198),

    .tertiary              = QColor(160, 232, 212),
    .on_tertiary           = QColor(0, 32, 33),
    .tertiary_container    = QColor(0, 77, 78),
    .on_tertiary_container = QColor(160, 232, 212),

    .error              = QColor(255, 180, 171),
    .on_error           = QColor(105, 0, 5),
    .error_container    = QColor(147, 0, 10),
    .on_error_container = QColor(255, 180, 171),

    .background         = QColor(30, 26, 10),
    .on_background      = QColor(240, 235, 220),
    .surface            = QColor(30, 26, 10),
    .on_surface         = QColor(240, 235, 220),
    .surface_variant    = QColor(74, 69, 50),
    .on_surface_variant = QColor(202, 196, 168),

    .outline         = QColor(146, 144, 137),
    .outline_variant = QColor(78, 75, 65),
    .shadow          = QColor(0, 0, 0),
    .scrim           = QColor(0, 0, 0),

    .inverse_surface    = QColor(240, 235, 220),
    .inverse_on_surface = QColor(51, 47, 30),
    .inverse_primary    = QColor(124, 94, 0),

    .surface_container_highest = QColor(50, 48, 41),
    .surface_container_high    = QColor(45, 43, 37),
    .surface_container         = QColor(40, 38, 32),
    .surface_container_low     = QColor(35, 33, 27),
    .surface_container_lowest  = QColor(30, 26, 10),
};

// 丰收金色主题包
constexpr auto kGoldenHarvestThemePack = ThemePack {
    .light = kGoldenHarvestLightColorScheme,
    .dark  = kGoldenHarvestDarkColorScheme,
};

}
