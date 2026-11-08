#pragma once

#include "creeper-qt/details/drawable/core.hh"

namespace creeper::drawable {

struct Ellipse {
    QSizeF size;
    QColor color = Qt::transparent;

    QColor outline_color = Qt::transparent;
    double outline_width = 0;

    friend auto dsl_invoke(details::draw_t, const Ellipse& self, QPainter& handler) {
        handler.save();
        {
            const auto rect = QRectF { QPointF {}, self.size };
            handler.setRenderHint(QPainter::Antialiasing);

            handler.setPen(Qt::NoPen);
            handler.setBrush(self.color);
            handler.drawEllipse(rect);

            if (self.outline_width > 0) {
                const auto half  = self.outline_width / 2;
                const auto inner = rect.adjusted(half, half, -half, -half);

                handler.setPen(QPen { self.outline_color, self.outline_width });
                handler.setBrush(Qt::NoBrush);
                handler.drawEllipse(inner);
            }
        }
        handler.restore();
    }

    friend auto dsl_invoke(details::size_t, const Ellipse& self) {
        return self.size;
    }
};

}
