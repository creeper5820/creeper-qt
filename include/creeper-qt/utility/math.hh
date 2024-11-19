#pragma once

#include <eigen3/Eigen/Eigen>

namespace creeper {

struct ArcBetweenLinesSolution {
    using Vector2d = Eigen::Vector2d;
    Eigen::Vector2d arcStartPoint;
    Eigen::Vector2d arcEndPoint;
    double angleStart;
    double angleEnd;

    void operator()(Vector2d e0, Vector2d e1, Vector2d e2, double radius) { }
};

}