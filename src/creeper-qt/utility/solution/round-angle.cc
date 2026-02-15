#include "round-angle.hh"
#include <eigen3/Eigen/Dense>

using namespace creeper;
using Eigen::Vector2d, std::numbers::pi;

static inline auto impl_round_angle_solution(
    RoundAngleSolution& solution, Vector2d e0, Vector2d e1, Vector2d e2, double radius) -> void {

    // solve the arc origin
    const auto v1 = Vector2d { e1 - e0 };
    const auto v2 = Vector2d { e2 - e0 };

    const auto dot = v1.x() * v2.x() + v1.y() * v2.y();
    const auto det = v1.x() * v2.y() - v1.y() * v2.x();

    const auto angle = std::abs(std::atan2(det, dot));
    const auto width = radius / std::tan(angle / 2);

    const auto point_begin = Vector2d { e0 + width * v1.normalized() };
    const auto point_end   = Vector2d { e0 + width * v2.normalized() };

    const auto origin = Vector2d { point_begin + radius * v1.unitOrthogonal() };

    solution.start = QPointF { point_begin.x(), point_begin.y() };
    solution.end   = QPointF { point_end.x(), point_end.y() };

    // solve the rect corners
    const auto v3          = Vector2d { e0 - origin }.normalized();
    const auto v4          = Vector2d { v3.unitOrthogonal() };
    const Vector2d corner0 = origin + Vector2d::UnitX() * radius + Vector2d::UnitY() * radius;
    const Vector2d corner1 = origin - Vector2d::UnitX() * radius - Vector2d::UnitY() * radius;
    solution.rect = QRectF { QPointF(corner1.x(), corner1.y()), QPointF(corner0.x(), corner0.y()) };

    // solve the arc angle
    // 角度计算时，注意Qt的系Y的正方向向下，但角度又是从X正方向逆时针开始计算,可谓混乱
    const auto angle_begin_vector = Vector2d { point_begin - origin };
    const auto angle_end_vector   = Vector2d { point_end - origin };

    const auto angle_end = std::atan2(-angle_end_vector.y(), angle_end_vector.x());

    solution.angle_begin  = std::atan2(-angle_begin_vector.y(), angle_begin_vector.x());
    solution.angle_length = angle_end - solution.angle_begin;

    if (solution.angle_length < -pi) solution.angle_length = 2 * pi + solution.angle_length;

    solution.angle_begin  = solution.angle_begin * 180 / pi;
    solution.angle_length = solution.angle_length * 180 / pi;
}

RoundAngleSolution::RoundAngleSolution(QPointF e0, QPointF e1, QPointF e2, double radius) noexcept {
    impl_round_angle_solution(
        *this, Eigen::Vector2d { e0.x(), e0.y() }, { e1.x(), e1.y() }, { e2.x(), e2.y() }, radius);
}
