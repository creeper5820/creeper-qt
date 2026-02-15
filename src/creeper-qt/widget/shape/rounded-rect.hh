#pragma once

#include "creeper-qt/utility/painter/helper.hh"
#include "creeper-qt/utility/wrapper/property.hh"
#include "creeper-qt/utility/wrapper/widget.hh"
#include "creeper-qt/widget/shape/shape.hh"

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

    void set_radius_top_left(double radius) { set_radius_nx_ny(radius); }

    void set_radius_top_right(double radius) { set_radius_px_ny(radius); }

    void set_radius_bottom_left(double radius) { set_radius_nx_py(radius); }

    void set_radius_bottom_right(double radius) { set_radius_px_py(radius); }

protected:
    void paintEvent(QPaintEvent* event) override {
        auto painter = QPainter { this };

        util::PainterHelper { painter }
            .set_render_hint(QPainter::Antialiasing)
            .rounded_rectangle( //
                background_, border_color_, border_width_, rect(),
                radius_nx_ny_, // tl: 左上
                radius_px_ny_, // tr: 右上
                radius_px_py_, // br: 右下
                radius_nx_py_  // bl: 左下
                )
            .done();
    }

private:
    double radius_nx_ny_ = 0;
    double radius_px_py_ = 0;
    double radius_nx_py_ = 0;
    double radius_px_ny_ = 0;
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

using RadiusTopLeft     = RadiusNxNy;
using RadiusTopRight    = RadiusPxNy;
using RadiusBottomLeft  = RadiusNxPy;
using RadiusBottomRight = RadiusPxPy;

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
