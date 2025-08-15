#pragma once

#include "creeper-qt/utility/painter/helper.hh"
#include "creeper-qt/utility/wrapper/property.hh"
#include "creeper-qt/widget/shape/shape.hh"
#include "creeper-qt/widget/widget.hh"

namespace creeper {
namespace rounded_rect::internal {
    class RoundedRect : public Shape {
    public:
        void set_radius_x(double radius_x) { radius_x_ = radius_x; }
        void set_radius_y(double radius_y) { radius_y_ = radius_y; }
        void set_radius(double radius) { radius_x_ = radius, radius_y_ = radius; }

    protected:
        void paintEvent(QPaintEvent* event) override {
            auto painter = QPainter { this };
            painter.setRenderHint(QPainter::Antialiasing);

            util::PainterHelper { painter }.rounded_rectangle(
                background_, border_color_, border_width_, rect(), radius_x_, radius_y_);
        }

    private:
        double radius_x_;
        double radius_y_;
    };
}
namespace rounded_rect::pro {
    using Property = common::Token<internal::RoundedRect>;

    // 通用属性
    using RadiusX = common::pro::RadiusX<Property>;
    using RadiusY = common::pro::RadiusY<Property>;
    using Radius  = common::pro::Radius<Property>;

    using Background = common::pro::Background<Property>;

    using BorderWidth = common::pro::BorderWidth<Property>;
    using BorderColor = common::pro::BorderColor<Property>;

    template <typename T>
    concept property_concept = std::derived_from<T, Property> || widget::pro::trait<T>;

    struct checker {
        template <class T> struct result {
            static constexpr auto v = property_concept<T>;
        };
    };

    using namespace widget::pro;
}

using RoundedRect = Declarative<rounded_rect::internal::RoundedRect, rounded_rect::pro::checker>;

}
