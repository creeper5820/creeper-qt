#pragma once

#include <qcolor.h>
#include <qfont.h>

namespace creeper::util::theme {

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
};

struct Typography {
    QFont body;
    QFont title;
    QFont button;
};

}
