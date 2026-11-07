#pragma once

#include "creeper-qt/details/token/sys/typography.hh"

#include <qfont.h>

namespace creeper {

struct Font : QFont {
    double line_height = 0;

    using QFont::QFont;

    Font() = default;

    Font(const QFont& font)
        : QFont { font } { }

    Font(const token::sys::typography::TypeScale& scale, const QFont& base = QFont { })
        : QFont { base }
        , line_height { scale.line_height.value } {
        QFont::setPixelSize(scale.size.px());
        QFont::setWeight(static_cast<QFont::Weight>(scale.weight));
        QFont::setLetterSpacing(QFont::AbsoluteSpacing, scale.tracking.value);
    }
};

}
