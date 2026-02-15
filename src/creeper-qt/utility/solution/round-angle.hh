#pragma once

#include <qpoint.h>
#include <qrect.h>

namespace creeper {

struct RoundAngleSolution {

    /// @brief 给定原点和端点，按逆时针方向计算圆弧
    /// @note 圆弧注意按照逆时针算
    /// @param e0 两切线交点
    /// @param e1 圆弧起始点切线
    /// @param e2 圆弧终点切线
    /// @param radius 半径
    RoundAngleSolution(QPointF e0, QPointF e1, QPointF e2, double radius) noexcept;

    QRectF rect;
    QPointF start;
    QPointF end;
    double angle_begin;
    double angle_length;
};

}
