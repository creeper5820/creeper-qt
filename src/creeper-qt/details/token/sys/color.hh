#pragma once

#include <qcolor.h>

namespace creeper {

struct Color : QColor {
    using QColor::QColor;

    Color() = default;

    Color(const QColor& color)
        : QColor { color } { }

    Color(const QColor& color, double opacity)
        : QColor { color } {
        QColor::setAlphaF(opacity);
    }
};

}
