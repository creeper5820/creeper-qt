#pragma once

#include "creeper-qt/details/api/scope/common.hh" // IWYU pragma: keep
#include "creeper-qt/details/api/scope/shape.hh"  // IWYU pragma: keep
#include "creeper-qt/details/api/scope/widget.hh" // IWYU pragma: keep
#include "creeper-qt/details/drawable/core.hh"
#include "creeper-qt/details/drawable/shape/rect.hh"
#include "creeper-qt/details/dsl/core.hh"
#include "creeper-qt/widget/shape/shape.hh"

namespace creeper {

class RoundedRect : public Shape, public DSL {
public:
    using Shape::Shape;

    explicit RoundedRect(auto&&... args) {
        construct_with(std::forward<decltype(args)>(args)...);
    }

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

    auto setRadiusTopLeft(double radius) -> void {
        setRadiusNxNy(radius);
    }

    auto setRadiusTopRight(double radius) -> void {
        setRadiusPxNy(radius);
    }

    auto setRadiusBottomLeft(double radius) -> void {
        setRadiusNxPy(radius);
    }

    auto setRadiusBottomRight(double radius) -> void {
        setRadiusPxPy(radius);
    }

protected:
    void paintEvent(QPaintEvent*) override {
        using namespace drawable;

        auto painter = QPainter { this };
        DrawHere {
            painter,
            Rect {
              .size  = size(),
              .color = background_,

              .outline_color = border_color_,
              .outline_width = border_width_,

              .radius_tl = radius_nx_ny_,
              .radius_tr = radius_px_ny_,
              .radius_br = radius_px_py_,
              .radius_bl = radius_nx_py_,
            },
        };
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
