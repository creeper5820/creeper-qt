#pragma once

#include "creeper-qt/utility/theme/theme.hh"

namespace creeper {

constexpr auto kBlueMikuLightColorScheme = ColorScheme {
    // 蓝色初音亮色
    .primary              = QColor(0, 89, 199),
    .on_primary           = QColor(255, 255, 255),
    .primary_container    = QColor(217, 226, 255),
    .on_primary_container = QColor(0, 26, 67),

    .secondary              = QColor(87, 94, 113),
    .on_secondary           = QColor(255, 255, 255),
    .secondary_container    = QColor(219, 226, 249),
    .on_secondary_container = QColor(20, 27, 44),

    .tertiary              = QColor(114, 85, 115),
    .on_tertiary           = QColor(255, 255, 255),
    .tertiary_container    = QColor(252, 215, 251),
    .on_tertiary_container = QColor(42, 19, 45),

    .error              = QColor(186, 26, 26),
    .on_error           = QColor(255, 255, 255),
    .error_container    = QColor(255, 218, 214),
    .on_error_container = QColor(65, 0, 2),

    .background         = QColor(254, 251, 255),
    .on_background      = QColor(27, 27, 31),
    .surface            = QColor(254, 251, 255),
    .on_surface         = QColor(27, 27, 31),
    .surface_variant    = QColor(225, 226, 236),
    .on_surface_variant = QColor(68, 70, 79),

    .outline         = QColor(117, 119, 128),
    .outline_variant = QColor(197, 198, 208),
    .shadow          = QColor(0, 0, 0),
    .scrim           = QColor(0, 0, 0),

    .inverse_surface    = QColor(48, 48, 52),
    .inverse_on_surface = QColor(242, 240, 244),
    .inverse_primary    = QColor(175, 198, 255),

    .surface_container_highest = QColor(254, 251, 255),
    .surface_container_high    = QColor(242, 243, 252),
    .surface_container         = QColor(234, 238, 251),
    .surface_container_low     = QColor(226, 233, 249),
    .surface_container_lowest  = QColor(224, 232, 248),
};
constexpr auto kBlueMikuDarkColorScheme = ColorScheme {
    // 蓝色初音暗色
    .primary              = QColor(175, 198, 255),
    .on_primary           = QColor(0, 45, 108),
    .primary_container    = QColor(0, 67, 152),
    .on_primary_container = QColor(217, 226, 255),

    .secondary              = QColor(191, 198, 220),
    .on_secondary           = QColor(41, 48, 66),
    .secondary_container    = QColor(63, 71, 89),
    .on_secondary_container = QColor(219, 226, 249),

    .tertiary              = QColor(223, 187, 222),
    .on_tertiary           = QColor(64, 39, 67),
    .tertiary_container    = QColor(89, 62, 90),
    .on_tertiary_container = QColor(252, 215, 251),

    .error              = QColor(255, 180, 171),
    .on_error           = QColor(105, 0, 5),
    .error_container    = QColor(147, 0, 10),
    .on_error_container = QColor(255, 180, 171),

    .background         = QColor(27, 27, 31),
    .on_background      = QColor(227, 226, 230),
    .surface            = QColor(27, 27, 31),
    .on_surface         = QColor(227, 226, 230),
    .surface_variant    = QColor(68, 70, 79),
    .on_surface_variant = QColor(197, 198, 208),

    .outline         = QColor(143, 144, 153),
    .outline_variant = QColor(68, 70, 79),
    .shadow          = QColor(0, 0, 0),
    .scrim           = QColor(0, 0, 0),

    .inverse_surface    = QColor(227, 226, 230),
    .inverse_on_surface = QColor(48, 48, 52),
    .inverse_primary    = QColor(0, 89, 199),

    .surface_container_highest = QColor(27, 27, 31),
    .surface_container_high    = QColor(34, 35, 42),
    .surface_container         = QColor(39, 40, 49),
    .surface_container_low     = QColor(43, 46, 56),
    .surface_container_lowest  = QColor(44, 47, 57),
};

constexpr auto kBlueMikuThemePack = ThemePack {
    .light = kBlueMikuLightColorScheme,
    .dark  = kBlueMikuDarkColorScheme,
};

}
