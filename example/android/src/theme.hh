#pragma once

#include "creeper-qt/utility/theme/theme.hh"

namespace creeper {

// Material 3 基线主题（Material Theme Builder 默认紫色）
constexpr auto kMaterialBaselineLightColorScheme = ColorScheme {
    .primary              = QColor(0x67, 0x50, 0xA4),
    .on_primary           = QColor(0xFF, 0xFF, 0xFF),
    .primary_container    = QColor(0xE9, 0xDD, 0xFF),
    .on_primary_container = QColor(0x21, 0x05, 0x54),

    .secondary              = QColor(0x62, 0x5B, 0x71),
    .on_secondary           = QColor(0xFF, 0xFF, 0xFF),
    .secondary_container    = QColor(0xE8, 0xDE, 0xF8),
    .on_secondary_container = QColor(0x1D, 0x19, 0x2B),

    .tertiary              = QColor(0x7D, 0x52, 0x64),
    .on_tertiary           = QColor(0xFF, 0xFF, 0xFF),
    .tertiary_container    = QColor(0xFF, 0xD8, 0xE7),
    .on_tertiary_container = QColor(0x31, 0x0C, 0x1C),

    .error              = QColor(0xB3, 0x26, 0x1E),
    .on_error           = QColor(0xFF, 0xFF, 0xFF),
    .error_container    = QColor(0xF9, 0xDE, 0xDA),
    .on_error_container = QColor(0x41, 0x0E, 0x0B),

    .background         = QColor(0xFE, 0xF7, 0xFF),
    .on_background      = QColor(0x1D, 0x1B, 0x20),
    .surface            = QColor(0xFE, 0xF7, 0xFF),
    .on_surface         = QColor(0x1D, 0x1B, 0x20),
    .surface_variant    = QColor(0xE7, 0xE0, 0xEC),
    .on_surface_variant = QColor(0x49, 0x45, 0x4F),

    .outline         = QColor(0x79, 0x74, 0x7E),
    .outline_variant = QColor(0xCA, 0xC4, 0xD0),
    .shadow          = QColor(0x00, 0x00, 0x00),
    .scrim           = QColor(0x00, 0x00, 0x00),

    .inverse_surface    = QColor(0x32, 0x2F, 0x35),
    .inverse_on_surface = QColor(0xF5, 0xEF, 0xF7),
    .inverse_primary    = QColor(0xD0, 0xBC, 0xFF),

    .surface_container_highest = QColor(0xE6, 0xE0, 0xE9),
    .surface_container_high    = QColor(0xEC, 0xE6, 0xF0),
    .surface_container         = QColor(0xF3, 0xED, 0xF7),
    .surface_container_low     = QColor(0xF7, 0xF2, 0xFA),
    .surface_container_lowest  = QColor(0xFF, 0xFF, 0xFF),
};

constexpr auto kMaterialBaselineDarkColorScheme = ColorScheme {
    .primary              = QColor(0xD0, 0xBC, 0xFF),
    .on_primary           = QColor(0x38, 0x1E, 0x72),
    .primary_container    = QColor(0x4F, 0x37, 0x8B),
    .on_primary_container = QColor(0xE9, 0xDD, 0xFF),

    .secondary              = QColor(0xCC, 0xC2, 0xDC),
    .on_secondary           = QColor(0x33, 0x2D, 0x41),
    .secondary_container    = QColor(0x4A, 0x44, 0x58),
    .on_secondary_container = QColor(0xE8, 0xDE, 0xF8),

    .tertiary              = QColor(0xEF, 0xB8, 0xCF),
    .on_tertiary           = QColor(0x49, 0x24, 0x36),
    .tertiary_container    = QColor(0x63, 0x3B, 0x4D),
    .on_tertiary_container = QColor(0xFF, 0xD8, 0xE7),

    .error              = QColor(0xF2, 0xB8, 0xB0),
    .on_error           = QColor(0x60, 0x14, 0x0E),
    .error_container    = QColor(0x8C, 0x1D, 0x18),
    .on_error_container = QColor(0xF9, 0xDE, 0xDA),

    .background         = QColor(0x14, 0x11, 0x18),
    .on_background      = QColor(0xE6, 0xE1, 0xE9),
    .surface            = QColor(0x14, 0x11, 0x18),
    .on_surface         = QColor(0xE6, 0xE1, 0xE9),
    .surface_variant    = QColor(0x49, 0x45, 0x4F),
    .on_surface_variant = QColor(0xCA, 0xC4, 0xD0),

    .outline         = QColor(0x93, 0x8F, 0x99),
    .outline_variant = QColor(0x49, 0x45, 0x4F),
    .shadow          = QColor(0x00, 0x00, 0x00),
    .scrim           = QColor(0x00, 0x00, 0x00),

    .inverse_surface    = QColor(0xE6, 0xE1, 0xE9),
    .inverse_on_surface = QColor(0x32, 0x2F, 0x35),
    .inverse_primary    = QColor(0x67, 0x50, 0xA4),

    .surface_container_highest = QColor(0x36, 0x31, 0x3B),
    .surface_container_high    = QColor(0x2B, 0x29, 0x30),
    .surface_container         = QColor(0x21, 0x1E, 0x24),
    .surface_container_low     = QColor(0x1D, 0x1B, 0x20),
    .surface_container_lowest  = QColor(0x0F, 0x0D, 0x13),
};

constexpr auto kMaterialBaselineThemePack = ThemePack {
    .light = kMaterialBaselineLightColorScheme,
    .dark  = kMaterialBaselineDarkColorScheme,
};

} // namespace creeper
