#pragma once

#include "creeper-qt/details/drawable/core.hh"

#include <qpainterpath.h>

#include <algorithm>

namespace creeper::drawable {

struct Rect {
    QSizeF size;
    QColor color = Qt::transparent;

    QColor outline_color = Qt::transparent;
    double outline_width = 0;

    double radius_tl = 0;
    double radius_tr = 0;
    double radius_br = 0;
    double radius_bl = 0;

    friend auto dsl_invoke(details::draw_t, const Rect& self, QPainter& handler) {
        handler.save();
        {
            const auto rect    = QRectF { QPointF {}, QSizeF { self.size } };
            const auto rounded = self.radius_tl != 0 || self.radius_tr != 0 || self.radius_br != 0
                || self.radius_bl != 0;
            handler.setRenderHint(QPainter::Antialiasing, rounded);

            handler.setPen(Qt::NoPen);
            handler.setBrush(self.color);
            if (rounded) {
                handler.drawPath(RectPath {
                  rect, self.radius_tl, self.radius_tr, self.radius_br, self.radius_bl });
            } else {
                handler.drawRect(rect);
            }

            if (self.outline_width > 0) {
                const auto half      = self.outline_width / 2;
                const auto inner     = rect.adjusted(half, half, -half, -half);
                const auto radius_tl = std::max(self.radius_tl - half, 0.);
                const auto radius_tr = std::max(self.radius_tr - half, 0.);
                const auto radius_br = std::max(self.radius_br - half, 0.);
                const auto radius_bl = std::max(self.radius_bl - half, 0.);
                const auto inner_rounded =
                    radius_tl != 0 || radius_tr != 0 || radius_br != 0 || radius_bl != 0;

                handler.setPen(QPen { self.outline_color, self.outline_width });
                handler.setBrush(Qt::NoBrush);
                if (inner_rounded) {
                    handler.drawPath(
                        RectPath { inner, radius_tl, radius_tr, radius_br, radius_bl });
                } else {
                    handler.drawRect(inner);
                }
            }
        }
        handler.restore();
    }

    friend auto dsl_invoke(details::size_t, const Rect& self) {
        return self.size;
    }

private:
    struct RectPath : QPainterPath {
        explicit RectPath(const QRectF& rect, qreal tl, qreal tr, qreal br, qreal bl) {
            const auto max_radius = std::min(rect.width(), rect.height()) / 2;

            const auto clamp = [=](qreal radius) {
                return std::clamp(radius, qreal { 0 }, max_radius);
            };
            tl = clamp(tl);
            tr = clamp(tr);
            br = clamp(br);
            bl = clamp(bl);

            moveTo(rect.topLeft() + QPointF { tl, 0 });
            lineTo(rect.topRight() - QPointF { tr, 0 });
            arcTo(QRectF { rect.topRight().x() - 2 * tr, rect.topRight().y(), 2 * tr, 2 * tr }, 90,
                -90);
            lineTo(rect.bottomRight() - QPointF { 0, br });
            arcTo(QRectF { rect.bottomRight().x() - 2 * br, rect.bottomRight().y() - 2 * br, 2 * br,
                    2 * br },
                0, -90);
            lineTo(rect.bottomLeft() + QPointF { bl, 0 });
            arcTo(QRectF { rect.bottomLeft().x(), rect.bottomLeft().y() - 2 * bl, 2 * bl, 2 * bl },
                270, -90);
            lineTo(rect.topLeft() + QPointF { 0, tl });
            arcTo(QRectF { rect.topLeft().x(), rect.topLeft().y(), 2 * tl, 2 * tl }, 180, -90);

            closeSubpath();
        }
    };
};

}
