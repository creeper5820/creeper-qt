#pragma once
#include "creeper-qt/utility/painter/common.hh"
#include <qpainterpath.h>

namespace creeper::painter::internal {

struct EraseRectangle : public CommonProps {
    auto operator()(qt::painter& painter) const noexcept {
        painter.save();

        painter.setCompositionMode(QPainter::CompositionMode_DestinationOut);

        painter.setBrush(Qt::black);
        painter.setPen(Qt::NoPen);
        painter.drawRect(rect());

        painter.restore();
    }
};

struct Rectangle : public CommonProps, ShapeProps {
    auto operator()(qt::painter& painter) const noexcept {
        painter.save();

        const auto rectangle = qt::rect { origin, size };

        painter.setBrush(container_color);
        painter.setPen(Qt::NoPen);
        painter.drawRect(rectangle);

        if (outline_width > 0) {
            const auto thickness = outline_width / 2;
            const auto inliner   = rectangle.adjusted(thickness, thickness, -thickness, -thickness);

            painter.setPen({ outline_color, outline_width });
            painter.setBrush(Qt::NoBrush);
            painter.drawRect(inliner);
        }

        painter.restore();
    }
};

struct RoundedRectangle : public CommonProps, ShapeProps {
    double radius_tl = 0;
    double radius_tr = 0;
    double radius_bl = 0;
    double radius_br = 0;

    auto set_radiuses(double r) {
        radius_tl = r;
        radius_tr = r;
        radius_bl = r;
        radius_br = r;
    }

    auto operator()(qt::painter& painter) const noexcept {
        painter.save();
        painter.setRenderHint(QPainter::Antialiasing);

        const auto rect = qt::rect { origin, size };
        const auto outline_shape =
            make_rounded_rect_path(rect, radius_tl, radius_tr, radius_br, radius_bl);

        painter.setPen(Qt::NoPen);
        painter.setBrush(container_color);
        painter.drawPath(outline_shape);

        if (outline_width > 0) {
            const auto thickness = outline_width;
            const auto inliner_f = [=](double r) { return std::max(r - thickness / 2, 0.); };
            const auto inliner_rect =
                rect.adjusted(thickness / 2, thickness / 2, -thickness / 2, -thickness / 2);
            const auto inliner_shape = make_rounded_rect_path(inliner_rect, inliner_f(radius_tl),
                inliner_f(radius_tr), inliner_f(radius_br), inliner_f(radius_bl));

            painter.setBrush(Qt::NoBrush);
            painter.setPen({ outline_color, outline_width });
            painter.drawPath(inliner_shape);
        }

        painter.restore();
    }
    static constexpr auto make_rounded_rect_path(
        const qt::rect& rect, qreal tl, qreal tr, qreal br, qreal bl) noexcept -> QPainterPath {

        auto path = QPainterPath {};

        const auto max_radius = std::min(rect.width(), rect.height()) / 2.0;
        const auto clamp      = [&](qreal r) -> qreal {
            return r < 0 ? max_radius : std::min(r, max_radius);
        };

        tl = clamp(tl);
        tr = clamp(tr);
        br = clamp(br);
        bl = clamp(bl);

        const auto Arc = [](qreal x, qreal y, qreal r,
                             int start_angle) -> std::tuple<qt::rect, int, int> {
            return { qt::rect(x, y, 2 * r, 2 * r), start_angle, -90 };
        };

        path.moveTo(rect.topLeft() + qt::point(tl, 0));
        path.lineTo(rect.topRight() - qt::point(tr, 0));
        const auto [tr_rect, tr_start, tr_span] =
            Arc(rect.topRight().x() - 2 * tr, rect.topRight().y(), tr, 90);
        path.arcTo(tr_rect, tr_start, tr_span);

        path.lineTo(rect.bottomRight() - qt::point(0, br));
        const auto [br_rect, br_start, br_span] =
            Arc(rect.bottomRight().x() - 2 * br, rect.bottomRight().y() - 2 * br, br, 0);
        path.arcTo(br_rect, br_start, br_span);

        path.lineTo(rect.bottomLeft() + qt::point(bl, 0));
        const auto [bl_rect, bl_start, bl_span] =
            Arc(rect.bottomLeft().x(), rect.bottomLeft().y() - 2 * bl, bl, 270);
        path.arcTo(bl_rect, bl_start, bl_span);

        path.lineTo(rect.topLeft() + qt::point(0, tl));
        const auto [tl_rect, tl_start, tl_span] =
            Arc(rect.topLeft().x(), rect.topLeft().y(), tl, 180);
        path.arcTo(tl_rect, tl_start, tl_span);

        path.closeSubpath();
        return path;
    }
};

struct Text : CommonProps {
    qt::string text;
    qt::font font;
    qt::color color = Qt::black;
    qt::text_option text_option;
    qt::real scale = 1.;

    auto operator()(qt::painter& painter) const noexcept {
        painter.save();
        painter.scale(scale, scale);

        const auto origin_rect = rect();
        const auto offset_x    = origin_rect.x() * (1.0 - scale);
        const auto center_y    = origin_rect.y() + origin_rect.height() / 2.0;
        const auto offset_y    = center_y * (1.0 - scale);
        painter.translate(offset_x, offset_y);

        painter.setBrush(Qt::NoBrush);
        painter.setPen(color);
        painter.setFont(font);

        const auto scaled_rect = qt::rect {
            origin_rect.x() / scale,
            origin_rect.y() / scale,
            origin_rect.width() / scale,
            origin_rect.height() / scale,
        };
        painter.drawText(scaled_rect, text, text_option);

        painter.restore();
    }
};

}
namespace creeper::painter {

/// Export Rounded Rectangle

using RadiusTL = SetterProp<common::pro::Token, double,
    [](auto& self, auto radius) { self.radius_tl = radius; }>;
using RadiusTR = SetterProp<common::pro::Token, double,
    [](auto& self, auto radius) { self.radius_tr = radius; }>;
using RadiusBL = SetterProp<common::pro::Token, double,
    [](auto& self, auto radius) { self.radius_bl = radius; }>;
using RadiusBR = SetterProp<common::pro::Token, double,
    [](auto& self, auto radius) { self.radius_br = radius; }>;
using Radiuses = SetterProp<common::pro::Token, double,
    [](auto& self, auto radius) { self.set_radiuses(radius); }>;

/// Export Text
using Text  = DerivedProp<common::pro::Token, qt::string,
     [](auto& self, const auto& text) { self.text = text; }>;
using Font  = DerivedProp<common::pro::Token, qt::font,
     [](auto& self, const auto& font) { self.font = font; }>;
using Color = DerivedProp<common::pro::Token, qt::color,
    [](auto& self, const auto& color) { self.color = color; }>;
using Scale = SetterProp<common::pro::Token, qt::real,
    [](auto& self, const auto& scale) { self.scale = scale; }>;

using TextOption = DerivedProp<common::pro::Token, qt::text_option,
    [](auto& self, const auto& option) { self.text_option = option; }>;

namespace Paint {
    using EraseRectangle = Declarative<internal::EraseRectangle, CheckerOr<common::pro::checker>>;
    using Rectangle      = Declarative<internal::Rectangle, CheckerOr<common::pro::checker>>;
    using RoundedRectangle =
        Declarative<internal::RoundedRectangle, CheckerOr<common::pro::checker>>;
    using Text = Declarative<internal::Text, CheckerOr<common::pro::checker>>;
}

}
