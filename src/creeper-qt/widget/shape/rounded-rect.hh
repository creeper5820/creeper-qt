#pragma once

#include "creeper-qt/utility/api/scope/common.hh"
#include "creeper-qt/utility/api/scope/shape.hh"

#include "creeper-qt/utility/api/scope/widget.hh"
#include "creeper-qt/utility/painter/helper.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/widget/shape/shape.hh"

namespace creeper {

class RoundedRect : public Shape, public DSL {
public:
    using Shape::Shape;

    explicit RoundedRect(auto&&... args) { construct_with(std::forward<decltype(args)>(args)...); }

    auto setRadius(double radius) -> void {
        radius_nx_ny_ = radius;
        radius_px_py_ = radius;
        radius_nx_py_ = radius;
        radius_px_ny_ = radius;
        update();
    }

    auto setRadiusNxNy(double radius) -> void {
        radius_nx_ny_ = radius;
        update();
    }
    auto setRadiusPxPy(double radius) -> void {
        radius_px_py_ = radius;
        update();
    }
    auto setRadiusNxPy(double radius) -> void {
        radius_nx_py_ = radius;
        update();
    }
    auto setRadiusPxNy(double radius) -> void {
        radius_px_ny_ = radius;
        update();
    }

    auto setRadiusTopLeft(double radius) -> void { setRadiusNxNy(radius); }

    auto setRadiusTopRight(double radius) -> void { setRadiusPxNy(radius); }

    auto setRadiusBottomLeft(double radius) -> void { setRadiusNxPy(radius); }

    auto setRadiusBottomRight(double radius) -> void { setRadiusPxPy(radius); }

protected:
    void paintEvent(QPaintEvent*) override {
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

namespace rounded_rect::pro {
    using RadiusTopLeft     = api::pro::RadiusNxNy;
    using RadiusTopRight    = api::pro::RadiusPxNy;
    using RadiusBottomLeft  = api::pro::RadiusNxPy;
    using RadiusBottomRight = api::pro::RadiusPxPy;

    using namespace api::scope::common;
    using namespace api::scope::shape;
    using namespace api::scope::widget;
}

}
