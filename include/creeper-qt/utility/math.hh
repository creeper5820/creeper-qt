#pragma once

#include <eigen3/Eigen/Eigen>

#include <iostream>
#include <numbers>

#include <qrect.h>

namespace creeper {

namespace internal {
    template <typename... Args> inline void log(Args... args) {
        ((std::cout << args << ' '), ...);
        std::cout << '\n';
    };
}

/// @brief 给定原点和端点，按逆时针方向计算圆弧
/// @note 圆弧注意按照逆时针算
/// @param e0 两切线交点
/// @param e1 圆弧起始点切线
/// @param e2 圆弧终点切线
/// @param radius 半径
struct RoundAngleSolution {
    using Vector2d = Eigen::Vector2d;
    RoundAngleSolution(Vector2d e0, Vector2d e1, Vector2d e2, double radius) {
        using Eigen::Vector2d, std::numbers::pi;
        // solve the arc origin
        const auto v1 = Vector2d { e1 - e0 };
        const auto v2 = Vector2d { e2 - e0 };
        const auto dot = v1.x() * v2.x() + v1.y() * v2.y();
        const auto det = v1.x() * v2.y() - v1.y() * v2.x();
        const auto angle = std::abs(std::atan2(det, dot));
        const auto width = radius / std::tan(angle / 2);
        const auto pointStart = Vector2d { e0 + width * v1.normalized() };
        const auto pointEnd = Vector2d { e0 + width * v2.normalized() };
        const auto origin = Vector2d { pointStart + radius * v1.unitOrthogonal() };
        start = QPointF { pointStart.x(), pointStart.y() };
        end = QPointF { pointEnd.x(), pointEnd.y() };

        // solve the rect corners
        const auto v3 = Vector2d { e0 - origin }.normalized();
        const auto v4 = Vector2d { v3.unitOrthogonal() };
        const Vector2d corner0 = origin + Vector2d::UnitX() * radius + Vector2d::UnitY() * radius;
        const Vector2d corner1 = origin - Vector2d::UnitX() * radius - Vector2d::UnitY() * radius;
        rect = QRectF { QPointF(corner1.x(), corner1.y()), QPointF(corner0.x(), corner0.y()) };

        // solve the arc angle
        // 角度计算时，注意Qt的系Y的正方向向下，但角度又是从X正方向逆时针开始计算,可谓混乱
        const auto angleStartVector = Vector2d { pointStart - origin };
        const auto angleEndVector = Vector2d { pointEnd - origin };
        const auto angleEnd = std::atan2(-angleEndVector.y(), angleEndVector.x());
        angleStart = std::atan2(-angleStartVector.y(), angleStartVector.x());
        angleLength = angleEnd - angleStart;
        if (angleLength < -pi) angleLength = 2 * pi + angleLength;

        angleStart = angleStart * 180 / pi;
        angleLength = angleLength * 180 / pi;
    }

    RoundAngleSolution(QPointF e0, QPointF e1, QPointF e2, double radius)
        : RoundAngleSolution(Eigen::Vector2d { e0.x(), e0.y() }, { e1.x(), e1.y() },
              { e2.x(), e2.y() }, radius) { }

    QRectF rect;
    QPointF start;
    QPointF end;
    double angleStart;
    double angleLength;
};

}