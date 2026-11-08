#pragma once

#include "creeper-qt/details/drawable/core.hh"

#include <qfont.h>
#include <qicon.h>
#include <qstring.h>

namespace creeper::drawable {

struct Icon {
    QIcon icon;

    QString text;
    QFont font;

    QSizeF size;
    QColor color = Qt::black;

    Qt::Alignment align = Qt::AlignCenter;
    QIcon::Mode mode    = QIcon::Normal;
    QIcon::State state  = QIcon::Off;

    friend auto dsl_invoke(details::draw_t, const Icon& self, QPainter& handler) {
        handler.save();
        {
            const auto rect = QRectF { QPointF { }, self.size };
            if (!self.text.isEmpty()) {
                handler.setRenderHint(QPainter::TextAntialiasing);
                handler.setPen(self.color);
                handler.setBrush(Qt::NoBrush);
                handler.setFont(self.font);
                handler.drawText(rect, self.align, self.text);
            } else {
                self.icon.paint(&handler, rect.toRect(), self.align, self.mode, self.state);
            }
        }
        handler.restore();
    }

    friend auto dsl_invoke(details::size_t, const Icon& self) {
        return self.size;
    }
};

}
