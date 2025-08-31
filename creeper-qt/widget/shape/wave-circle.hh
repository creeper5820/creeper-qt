#pragma once

#include "creeper-qt/utility/solution/round-angle.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/property.hh"
#include "creeper-qt/widget/shape/shape.hh"
#include "creeper-qt/widget/widget.hh"

#include <cmath>
#include <qpainterpath.h>
#include <ranges>

namespace creeper::wave_circle::internal {

class WaveCircle : public Shape {
public:
    auto set_flange_number(uint8_t number) noexcept {
        generate_request_ = true;
        flange_number_    = number;
    }
    auto set_flange_radius(double radius) noexcept {
        generate_request_ = true;
        flange_radius_    = radius;
    }
    auto set_overall_radius(double radius) noexcept {
        generate_request_ = true;
        overall_radius_   = radius;
    }
    auto set_protruding_ratio(double ratio) noexcept {
        generate_request_ = true;
        protruding_ratio_ = ratio;
    }

protected:
    auto paintEvent(QPaintEvent*) -> void override {
        if (generate_request_) generate_path();

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

    auto generate_path() noexcept -> void {

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

        auto begin  = QPointF {};
        path_cache_ = QPainterPath {};
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
}
namespace creeper::wave_circle::pro {

using Token = common::Token<internal::WaveCircle>;

using Background  = common::pro::Background<Token>;
using BorderWidth = common::pro::BorderWidth<Token>;
using BorderColor = common::pro::BorderColor<Token>;

using FlangeNumber =
    SetterProp<Token, uint8_t, [](auto& self, const auto& v) { self.set_flange_number(v); }>;

using FlangeRadius =
    SetterProp<Token, double, [](auto& self, const auto& v) { self.set_flange_radius(v); }>;

using OverallRadius =
    SetterProp<Token, double, [](auto& self, const auto& v) { self.set_overall_radius(v); }>;

using ProtrudingRatio =
    SetterProp<Token, double, [](auto& self, const auto& v) { self.set_protruding_ratio(v); }>;

template <class T>
concept trait = std::derived_from<T, Token>;

CREEPER_DEFINE_CHECKER(trait);
using namespace widget::pro;
}
namespace creeper {

using WaveCircle = Declarative<wave_circle::internal::WaveCircle,
    CheckerOr<wave_circle::pro::checker, widget::pro::checker>>;

}
