#pragma once

#include "creeper-qt/utility/api/scope/common.hh" // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/shape.hh"  // IWYU pragma: keep
#include "creeper-qt/utility/api/scope/widget.hh" // IWYU pragma: keep
#include "creeper-qt/utility/solution/round-angle.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/utility/wrapper/forward_prop.hh"
#include "creeper-qt/widget/shape/shape.hh"

#include <qpainterpath.h>

#include <cmath>
#include <numbers>
#include <ranges>

namespace creeper {

class WaveCircle : public Shape, public DSL {
public:
    using Shape::Shape;

    explicit WaveCircle(auto&&... args) { construct_with(std::forward<decltype(args)>(args)...); }

    auto setFlangeNumber(uint8_t number) noexcept -> void {
        generate_request_ = true;
        flange_number_    = number;
    }
    auto setFlangeRadius(double radius) noexcept -> void {
        generate_request_ = true;
        flange_radius_    = radius;
    }
    auto setOverallRadius(double radius) noexcept -> void {
        generate_request_ = true;
        overall_radius_   = radius;
    }
    auto setProtrudingRatio(double ratio) noexcept -> void {
        generate_request_ = true;
        protruding_ratio_ = ratio;
    }

protected:
    auto paintEvent(QPaintEvent*) -> void override {
        if (generate_request_) generatePath();

        auto painter = QPainter { this };
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setOpacity(1);
        painter.setBrush({ background_ });
        painter.setPen(QPen {
            border_color_,
            border_width_,
            Qt::SolidLine,
            Qt::RoundCap,
        });
        painter.drawPath(path_cache_);
    }
    auto resizeEvent(QResizeEvent* e) -> void override {
        Shape::resizeEvent(e);
        generate_request_ = true;
    }

private:
    bool generate_request_ = true;
    QPainterPath path_cache_;

    int8_t flange_number_    = 12;
    double flange_radius_    = 10;
    double overall_radius_   = 100;
    double protruding_ratio_ = 0.8;

    auto generatePath() noexcept -> void {

        const auto center = QPointF(width() / 2., height() / 2.);
        const auto step   = 2 * std::numbers::pi / flange_number_;
        const auto radius = std::min(overall_radius_, std::min<double>(width(), height()));

        std::vector<QPointF> outside(flange_number_ + 2), inside(flange_number_ + 2);
        for (auto&& [index, point] : std::views::enumerate(std::views::zip(outside, inside))) {
            auto& [outside, inside] = point;
            outside.setX(radius * std::cos(-index * step));
            outside.setY(radius * std::sin(-index * step));
            inside.setX(protruding_ratio_ * radius * std::cos(double(-index + 0.5) * step));
            inside.setY(protruding_ratio_ * radius * std::sin(double(-index + 0.5) * step));
        }

        auto begin  = QPointF { };
        path_cache_ = QPainterPath { };
        for (int index = 0; index < flange_number_; index++) {
            const auto convex  = RoundAngleSolution(center + outside[index], center + inside[index],
                center + inside[index + 1], flange_radius_);
            const auto concave = RoundAngleSolution(center + inside[index + 1],
                center + outside[index + 1], center + outside[index], flange_radius_);
            if (index == 0) begin = convex.start, path_cache_.moveTo(begin);
            path_cache_.lineTo(convex.start);
            path_cache_.arcTo(convex.rect, convex.angle_begin, convex.angle_length);
            path_cache_.lineTo(concave.end);
            path_cache_.arcTo(
                concave.rect, concave.angle_begin + concave.angle_length, -concave.angle_length);
        }
        path_cache_.lineTo(begin);
    }
};

namespace wave_circle::pro {
    using FlangeNumber    = ForwardProp<&WaveCircle::setFlangeNumber>;
    using FlangeRadius    = ForwardProp<&WaveCircle::setFlangeRadius>;
    using OverallRadius   = ForwardProp<&WaveCircle::setOverallRadius>;
    using ProtrudingRatio = ForwardProp<&WaveCircle::setProtrudingRatio>;

    using namespace api::scope::common;
    using namespace api::scope::shape;
    using namespace api::scope::widget;
}

}
