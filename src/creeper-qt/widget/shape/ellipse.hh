#pragma once

#include "creeper-qt/details/drawable/core.hh"
#include "creeper-qt/details/drawable/shape/ellipse.hh"
#include "creeper-qt/details/dsl/core.hh"
#include "creeper-qt/utility/api/scope/common.hh" // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/shape.hh"  // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/widget.hh" // IWYU pragma: keep
#include "creeper-qt/widget/shape/shape.hh"

namespace creeper {

class Ellipse : public Shape, public DSL {
public:
    using Shape::Shape;

    explicit Ellipse(auto&&... args) {
        construct_with(std::forward<decltype(args)>(args)...);
    }

protected:
    auto paintEvent(QPaintEvent*) -> void override {
        using namespace drawable;

        auto painter = QPainter { this };
        DrawHere {
            painter,
            drawable::Ellipse {
              .size  = size(),
              .color = background_,

              .outline_color = border_color_,
              .outline_width = border_width_,
            },
        };
    }
};

namespace ellipse::pro {
    using namespace api::scope::common;
    using namespace api::scope::shape;
    using namespace api::scope::widget;
}

}
