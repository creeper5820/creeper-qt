#pragma once

#include "utility/wrapper/property.hh"
#include "widget/shape/shape.hh"
#include "widget/widget.hh"

namespace creeper {

namespace internal {

    class RoundedRect : public Shape {
    public:
        void set_radius_x(double radius_x) { radius_x_ = radius_x; }
        void set_radius_y(double radius_y) { radius_y_ = radius_y; }
        void set_radius(double radius) { radius_x_ = radius, radius_y_ = radius; }

    protected:
        void paintEvent(QPaintEvent* event) override {
            auto painter = QPainter { this };
            painter.setRenderHint(QPainter::Antialiasing);

            const auto inliner_border_rectangle = rect().adjusted(
                border_width_ / 2, border_width_ / 2, -border_width_ / 2, -border_width_ / 2);
            const auto inliner_outside_rectangle
                = rect().adjusted(border_width_, border_width_, -border_width_, -border_width_);

            painter.setBrush(Qt::NoBrush);
            painter.setPen(QPen { border_color_, border_width_ });
            painter.drawRoundedRect(inliner_border_rectangle,
                std::max(radius_x_ - border_width_ / 2, 0.),
                std::max(radius_y_ - border_width_ / 2, 0.));

            painter.setBrush(QBrush { background_ });
            painter.setPen(Qt::NoPen);
            painter.drawRoundedRect(inliner_outside_rectangle,
                std::max(radius_x_ - border_width_, 0.), std::max(radius_y_ - border_width_, 0.));
        }

    private:
        double radius_x_;
        double radius_y_;
    };

}

namespace pro::rounded_rect {

    using Property = InternalProperty<internal::RoundedRect>;

    template <typename T>
    concept property_concept = std::derived_from<T, Property> || pro::widget::property_concept<T>;

    // 通用属性
    using RadiusX = common::RadiusX<internal::RoundedRect, Property>;
    using RadiusY = common::RadiusY<internal::RoundedRect, Property>;
    using Radius = common::Radius<internal::RoundedRect, Property>;

    using Background = common::Background<internal::RoundedRect, Property>;

    using BorderWidth = common::BorderWidth<internal::RoundedRect, Property>;
    using BorderColor = common::BorderColor<internal::RoundedRect, Property>;

    using namespace pro::widget;
}

class RoundedRect : public internal::RoundedRect {
    CREEPER_DEFINE_CONSTROCTOR(RoundedRect, pro::rounded_rect);
    using internal::RoundedRect::RoundedRect;
};

}
