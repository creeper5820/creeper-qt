#pragma once

#include "creeper-qt/utility/wrapper/property.hh"
#include "creeper-qt/widget/shape/shape.hh"
#include "creeper-qt/widget/widget.hh"
#include <qpainter.h>
#include <qpainterpath.h>

namespace creeper::rounded_rect::internal {

class RoundedRect : public Shape {
public:
    void set_radius(double radius) {
        radius_nx_ny_ = radius;
        radius_px_py_ = radius;
        radius_nx_py_ = radius;
        radius_px_ny_ = radius;
        update();
    }

    void set_radius_nx_ny(double radius) {
        radius_nx_ny_ = radius;
        update();
    }
    void set_radius_px_py(double radius) {
        radius_px_py_ = radius;
        update();
    }
    void set_radius_nx_py(double radius) {
        radius_nx_py_ = radius;
        update();
    }
    void set_radius_px_ny(double radius) {
        radius_px_ny_ = radius;
        update();
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        auto painter = QPainter { this };
        painter.setRenderHint(QPainter::Antialiasing);
        painter.fillPath(path_, background_);

        if (border_width_ > 0) {
            auto pen = QPen { border_color_, border_width_ };
            painter.setPen(pen);
            painter.drawPath(path_);
        }
    }

    void resizeEvent(QResizeEvent* event) override {
        path_ = make_rounded_rect_path(
            rect(), radius_nx_ny_, radius_px_ny_, radius_px_py_, radius_nx_py_);

        Shape::resizeEvent(event);
    }

private:
    double radius_nx_ny_ = 0;
    double radius_px_py_ = 0;
    double radius_nx_py_ = 0;
    double radius_px_ny_ = 0;

    QPainterPath path_ =
        make_rounded_rect_path(rect(), radius_nx_ny_, radius_px_ny_, radius_px_py_, radius_nx_py_);

    static auto make_rounded_rect_path(
        const QRectF& rect, qreal tl, qreal tr, qreal br, qreal bl) noexcept -> QPainterPath {

        auto path = QPainterPath {};

        const auto half_width  = rect.width() / 2.0;
        const auto half_height = rect.height() / 2.0;

        const auto max_radius = std::min(half_width, half_height);

        tl = tl < 0 ? max_radius : std::min(tl, max_radius);
        tr = tr < 0 ? max_radius : std::min(tr, max_radius);
        br = br < 0 ? max_radius : std::min(br, max_radius);
        bl = bl < 0 ? max_radius : std::min(bl, max_radius);

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

        return path;
    }
};

}
namespace creeper::rounded_rect::pro {
using Token = common::Token<internal::RoundedRect>;

// 通用属性
using Radius = common::pro::Radius<Token>;

using RadiusPxPy = common::pro::RadiusPxPy<Token>;
using RadiusNxNy = common::pro::RadiusNxNy<Token>;
using RadiusPxNy = common::pro::RadiusPxNy<Token>;
using RadiusNxPy = common::pro::RadiusNxPy<Token>;

using Background = common::pro::Background<Token>;

using BorderWidth = common::pro::BorderWidth<Token>;
using BorderColor = common::pro::BorderColor<Token>;

template <typename T>
concept trait = std::derived_from<T, Token>;

CREEPER_DEFINE_CHECKER(trait)
using namespace widget::pro;
}
namespace creeper {

using RoundedRect = Declarative<rounded_rect::internal::RoundedRect,
    CheckerOr<rounded_rect::pro::checker, widget::pro::checker>>;

}
