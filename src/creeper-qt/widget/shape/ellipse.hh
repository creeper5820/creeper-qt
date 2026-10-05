#pragma once

#include "creeper-qt/utility/api/scope/common.hh" // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/shape.hh"  // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/widget.hh" // IWYU pragma: keep
#include "creeper-qt/utility/painter/helper.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/widget/shape/shape.hh"

namespace creeper {

class Ellipse : public Shape, public DSL {
public:
    using Shape::Shape;

    explicit Ellipse(auto&&... args) { construct_with(std::forward<decltype(args)>(args)...); }

protected:
    auto paintEvent(QPaintEvent*) -> void override {
        auto painter = QPainter { this };
        painter.setRenderHint(QPainter::Antialiasing);

        util::PainterHelper { painter }.ellipse(background_, border_color_, border_width_, rect());
    }
};

namespace ellipse::pro {
    using namespace api::scope::common;
    using namespace api::scope::shape;
    using namespace api::scope::widget;
}

}
