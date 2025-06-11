#pragma once

#include <qpainter.h>

namespace creeper::util {

/// @brief 隐藏冗杂的细节，解放命令式的绘图调用
class PainterHelper {
public:
    explicit PainterHelper(QPainter& painter)
        : painter(painter) { }

    inline void done() { /* 充当语气助词的函数 */ }

    inline PainterHelper& apply(std::function<void(QPainter&)> function) {
        function(painter);
        return *this;
    }

public:
    inline PainterHelper& set_brush(const QBrush& brush) {
        painter.setBrush(brush);
        return *this;
    }

    inline PainterHelper& set_pen(const QPen& pen) {
        painter.setPen(pen);
        return *this;
    }

    inline PainterHelper& set_opacity(double opacity) {
        painter.setOpacity(opacity);
        return *this;
    }

    inline PainterHelper& set_render_hint(QPainter::RenderHint hint, bool on = true) {
        painter.setRenderHint(hint, on);
        return *this;
    }

    inline PainterHelper& set_render_hints(QPainter::RenderHints hint, bool on = true) {
        painter.setRenderHints(hint, on);
        return *this;
    }

    inline PainterHelper& set_clip_path(
        const QPainterPath& path, Qt::ClipOperation operation = Qt::ReplaceClip) {
        painter.setClipPath(path, operation);
        return *this;
    }

public:
    inline PainterHelper& ellipse(const QColor& background, const QColor& border_color,
        double border_width, const QRectF& rect) {

        const auto inliner_border_rectangle = rect.adjusted(
            border_width / 2, border_width / 2, -border_width / 2, -border_width / 2);
        const auto inliner_outside_rectangle
            = rect.adjusted(border_width, border_width, -border_width, -border_width);

        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen { border_color, border_width });
        painter.drawEllipse(inliner_border_rectangle);

        painter.setBrush(QBrush { background });
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(inliner_outside_rectangle);

        return *this;
    }
    inline PainterHelper& ellipse(const QColor& background, const QColor& border_color,
        double border_width, const QPointF& origin, double radius_x, double radius_y) {

        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen { border_color, border_width });
        painter.drawEllipse(origin, radius_x - border_width / 2, radius_y - border_width / 2);

        painter.setBrush(QBrush { background });
        painter.setPen(Qt::NoPen);
        painter.drawEllipse(origin, radius_x, radius_y);

        return *this;
    }

    inline PainterHelper& rectangle(const QColor& background, const QColor& border_color,
        double border_width, const QRectF& rect) {

        const auto inliner_border_rectangle = rect.adjusted(
            border_width / 2, border_width / 2, -border_width / 2, -border_width / 2);
        const auto inliner_outside_rectangle
            = rect.adjusted(border_width, border_width, -border_width, -border_width);

        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen { border_color, border_width });
        painter.drawRect(inliner_border_rectangle);

        painter.setBrush(QBrush { background });
        painter.setPen(Qt::NoPen);
        painter.drawRect(inliner_outside_rectangle);

        return *this;
    }

    inline PainterHelper& rounded_rectangle(const QColor& background, const QColor& border_color,
        double border_width, const QRectF& rect, double radius_x, double radius_y) {

        const auto inliner_border_rectangle = rect.adjusted(
            border_width / 2, border_width / 2, -border_width / 2, -border_width / 2);
        const auto inliner_outside_rectangle
            = rect.adjusted(border_width, border_width, -border_width, -border_width);

        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen { border_color, border_width });
        painter.drawRoundedRect(inliner_border_rectangle, std::max(radius_x - border_width / 2, 0.),
            std::max(radius_y - border_width / 2, 0.));

        painter.setBrush(QBrush { background });
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(inliner_outside_rectangle, std::max(radius_x - border_width, 0.),
            std::max(radius_y - border_width, 0.));

        return *this;
    }

private:
    QPainter& painter;
};
}
