#pragma once

#include "utility/animation/core.hh"

#include <eigen3/Eigen/Dense>

namespace creeper::util::animation {

using RGBA = Eigen::Vector4d;
inline Eigen::Vector4d lerp(const Eigen::Vector4d& v1, const Eigen::Vector4d& v2, double t) {
    return (1.0 - t) * v1 + t * v2;
}

struct GradientColor final : IAnimation {
public:
    double ratio;
    RGBA begin;
    RGBA final;

    std::shared_ptr<QColor> color;
    std::shared_ptr<bool> interrupt;

    bool update(const QPaintEvent&) { return true; }
};

}
