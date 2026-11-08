#pragma once

#include "creeper-qt/details/drawable/core.hh"

namespace creeper::drawable {

struct Line {
    QPointF point;
    QColor color;
    double width = 0;

    friend auto dsl_invoke(details::draw_t, const Line& line, QPainter& handler) {
        handler.save();
        {
            handler.setPen(QPen { line.color, line.width });
            handler.setBrush(Qt::NoBrush);
            handler.drawLine(QPoint { }, line.point);
        }
        handler.restore();
    }

    friend auto dsl_invoke(details::size_t, const Line& line) {
        return QSizeF {
            std::abs(line.point.x()),
            std::abs(line.point.y()),
        };
    }
};

}
