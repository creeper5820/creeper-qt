#pragma once

#include <qpainter.h>
#include <qpainterpath.h>

namespace creeper::util {

/// @brief 隐藏冗杂的细节，解放命令式的绘图调用
class PainterHelper {
public:
    using Renderer = std::function<void(QPainter&)>;

    explicit PainterHelper(QPainter& painter)
        : painter(painter) { }

    inline void done() { }

    inline PainterHelper& apply(const Renderer& renderer) {
        renderer(painter);
        return *this;
    }

    inline PainterHelper& apply(const std::ranges::range auto& renderers)
        requires std::same_as<std::ranges::range_value_t<decltype(renderers)>, Renderer>
    {
        for (const auto& renderer : renderers)
            renderer(painter);
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

        brush_only({ background }).drawEllipse(rect);

        const auto half = border_width / 2;
        if (border_width != 0)
            pen_only({ border_color, border_width })
                .drawEllipse(rect.adjusted(half, half, -half, -half));

        return *this;
    }
    inline PainterHelper& ellipse(const QColor& background, const QColor& border_color,
        double border_width, const QPointF& origin, double radius_x, double radius_y) {

        brush_only({ background }).drawEllipse(origin, radius_x, radius_y);

        if (border_width != 0)
            pen_only({ border_color, border_width })
                .drawEllipse(origin, radius_x - border_width / 2, radius_y - border_width / 2);

        return *this;
    }

    inline PainterHelper& rectangle(const QColor& background, const QColor& border_color,
        double border_width, const QRectF& rect) {

        brush_only({ background }).drawRect(rect);

        if (border_width == 0) return *this;

        const auto inliner_border_rectangle =
            rect.adjusted(border_width / 2, border_width / 2, -border_width / 2, -border_width / 2);

        pen_only({ border_color, border_width }).drawRect(inliner_border_rectangle);

        return *this;
    }

    inline PainterHelper& rounded_rectangle(const QColor& background, const QColor& border_color,
        double border_width, const QRectF& rect, double radius_x, double radius_y) {

        brush_only({ background }).drawRoundedRect(rect, radius_x, radius_y);

        if (border_width == 0) return *this;
        const auto inliner_border_rectangle =
            rect.adjusted(border_width / 2, border_width / 2, -border_width / 2, -border_width / 2);

        pen_only({ border_color, border_width })
            .drawRoundedRect(inliner_border_rectangle, std::max(radius_x - border_width / 2, 0.),
                std::max(radius_y - border_width / 2, 0.));

        return *this;
    }

    inline PainterHelper& rounded_rectangle(const QColor& background, const QColor& border_color,
        double border_width, const QRectF& rect, double tl, double tr, double br, double bl) {

        const auto path = make_rounded_rect_path(rect, tl, tr, br, bl);
        brush_only({ background }).drawPath(path);

        if (border_width == 0) return *this;

        const auto inliner = [=](double r) { return std::max(r - border_width / 2, 0.); };
        const auto inliner_border_rectangle =
            rect.adjusted(border_width / 2, border_width / 2, -border_width / 2, -border_width / 2);
        const auto inliner_path = make_rounded_rect_path(
            inliner_border_rectangle, inliner(tl), inliner(tr), inliner(br), inliner(bl));

        pen_only({ border_color, border_width }).drawPath(inliner_path);

        return *this;
    }

    // Pen 是以路径为中心来绘制图片，有绘出 rect 导致画面被裁切的可能，由于是 path 类型，不好做限制
    inline PainterHelper& path(const QColor& background, const QColor& border_color,
        double border_width, const QPainterPath& path) {
        brush_only({ background }).drawPath(path);
        if (border_width != 0) pen_only({ border_color, border_width }).drawPath(path);
        return *this;
    }

    inline PainterHelper& pixmap(const QPixmap& pixmap, const QRectF& dst, const QRectF& src) {
        painter.drawPixmap(dst, pixmap, src);
        return *this;
    }

    inline PainterHelper& simple_text(const QString& text, const QFont& font, const QColor& color,
        const QRectF& rect, Qt::Alignment alignment) {
        painter.setRenderHint(QPainter::TextAntialiasing);
        painter.setFont(font);
        painter.setBrush(Qt::NoBrush);
        painter.setPen({ color });
        painter.drawText(rect, alignment, text);
        return *this;
    }

private:
    QPainter& painter;

    QPainter& pen_only(const QPen& pen) {
        painter.setBrush(Qt::NoBrush);
        painter.setPen(pen);
        return painter;
    }
    QPainter& brush_only(const QBrush& brush) {
        painter.setBrush(brush);
        painter.setPen(Qt::NoPen);
        return painter;
    }

    static auto make_rounded_rect_path(
        const QRectF& rect, qreal tl, qreal tr, qreal br, qreal bl) noexcept -> QPainterPath {

        auto path = QPainterPath {};

        const auto half_width  = rect.width() / 2.0;
        const auto half_height = rect.height() / 2.0;

        const auto max_radius = std::min(half_width, half_height);

        const auto clamp_radius = [&](qreal r) {
            return r < 0 ? max_radius : std::min(r, max_radius);
        };
        tl = clamp_radius(tl);
        tr = clamp_radius(tr);
        br = clamp_radius(br);
        bl = clamp_radius(bl);

        path.moveTo(rect.topLeft() + QPointF(tl, 0));

        path.lineTo(rect.topRight() - QPointF(tr, 0));
        path.arcTo(
            QRectF(rect.topRight().x() - 2 * tr, rect.topRight().y(), 2 * tr, 2 * tr), 90, -90);

        path.lineTo(rect.bottomRight() - QPointF(0, br));
        path.arcTo(QRectF(rect.bottomRight().x() - 2 * br, rect.bottomRight().y() - 2 * br, 2 * br,
                       2 * br),
            0, -90);

        path.lineTo(rect.bottomLeft() + QPointF(bl, 0));
        path.arcTo(QRectF(rect.bottomLeft().x(), rect.bottomLeft().y() - 2 * bl, 2 * bl, 2 * bl),
            270, -90);

        path.lineTo(rect.topLeft() + QPointF(0, tl));
        path.arcTo(QRectF(rect.topLeft().x(), rect.topLeft().y(), 2 * tl, 2 * tl), 180, -90);

        path.closeSubpath();

        return path;
    }
};
}
