#pragma once

#include <eigen3/Eigen/Eigen>

#include <numbers>

#include <qrect.h>

namespace creeper {

/// @brief 给定原点和端点1和端点2，按逆时针计算圆弧信息
/// @param e0 原点
/// @param e1 端点1
/// @param e2 端点2
/// @param radius 半径
struct ArcBetweenLinesSolution {
    ArcBetweenLinesSolution(
        Eigen::Vector2d e0, Eigen::Vector2d e1, Eigen::Vector2d e2, double radius) {
        using Eigen::Vector2d, std::numbers::pi;
        // solve the arc origin
        const auto v1 = Vector2d { e1 - e0 };
        const auto v2 = Vector2d { e2 - e0 };
        const auto dot = v1.x() * v2.x() + v1.y() * v2.y();
        const auto det = v1.x() * v2.y() - v1.y() * v2.x();
        const auto angle = std::abs(std::atan2(det, dot));
        const auto width = radius / std::tan(angle / 2);
        const auto start1 = Vector2d { e0 + width * v1.normalized() };
        const auto start2 = Vector2d { e0 + width * v2.normalized() };
        const auto origin = Vector2d { start1 - radius * v1.unitOrthogonal() };

        pointStart = QPointF { start2.x(), start2.y() };

        // solve the arc angle
        angleStart = std::atan2(v2.unitOrthogonal().y(), v2.unitOrthogonal().x());
        angleLength = std::atan2(v1.unitOrthogonal().y(), v1.unitOrthogonal().x()) - angleStart;

        angleStart = angleStart * 180 / pi;
        angleLength = angleLength * 180 / pi;

        // solve the rect corners
        const auto v3 = Vector2d { e0 - origin }.normalized();
        const auto v4 = Vector2d { v3.unitOrthogonal() };
        const Vector2d corner0 = origin + Vector2d::UnitX() * radius + Vector2d::UnitY() * radius;
        const Vector2d corner1 = origin - Vector2d::UnitX() * radius - Vector2d::UnitY() * radius;

        rect = QRectF { QPointF(corner1.x(), corner1.y()), QPointF(corner0.x(), corner0.y()) };
    }
    ArcBetweenLinesSolution(QPointF e0, QPointF e1, QPointF e2, double radius)
        : ArcBetweenLinesSolution(Eigen::Vector2d { e0.x(), e0.y() }, { e1.x(), e1.y() },
              { e2.x(), e2.y() }, radius) { }

    QRectF rect;
    QPointF pointStart;
    double angleStart;
    double angleLength;
};

}