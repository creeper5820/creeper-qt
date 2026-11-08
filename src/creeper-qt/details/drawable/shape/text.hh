#pragma once

#include "creeper-qt/details/drawable/core.hh"

#include <qfont.h>
#include <qstring.h>

namespace creeper::drawable {

struct Text {
    QString text;
    QFont font;

    QSizeF size;
    QColor color = Qt::black;

    Qt::Alignment align = Qt::AlignCenter;

    friend auto dsl_invoke(details::draw_t, const Text& self, QPainter& handler) {
        handler.save();
        {
            handler.setRenderHint(QPainter::TextAntialiasing);
            handler.setPen(self.color);
            handler.setBrush(Qt::NoBrush);
            handler.setFont(self.font);
            handler.drawText(QRectF { QPointF {}, self.size }, self.align, self.text);
        }
        handler.restore();
    }

    friend auto dsl_invoke(details::size_t, const Text& self) {
        return self.size;
    }
};

}
