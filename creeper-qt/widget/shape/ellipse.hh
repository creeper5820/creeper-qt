#pragma once

#include "creeper-qt/utility/painter/helper.hh"
#include "creeper-qt/utility/wrapper/property.hh"
#include "creeper-qt/widget/shape/shape.hh"
#include "creeper-qt/widget/widget.hh"

namespace creeper {
namespace ellipse::internal {
    class Ellipse : public Shape {
    protected:
        void paintEvent(QPaintEvent* event) override {
            auto painter = QPainter { this };
            painter.setRenderHint(QPainter::Antialiasing);

            util::PainterHelper { painter }.ellipse(
                background_, border_color_, border_width_, rect());
        }
    };
}
namespace ellipse::pro {
    using Token = common::Token<internal::Ellipse>;

    template <typename T>
    concept property_concept = std::derived_from<T, Token> || widget::pro::property_concept<T>;

    // 通用属性
    using Background = common::pro::Background<Token>;

    using BorderWidth = common::pro::BorderWidth<Token>;
    using BorderColor = common::pro::BorderColor<Token>;

    using namespace widget::pro;
}
class Ellipse : public ellipse::internal::Ellipse {
    CREEPER_DEFINE_CONSTROCTOR(Ellipse, ellipse::pro);
    using ellipse::internal::Ellipse::Ellipse;
};
}
