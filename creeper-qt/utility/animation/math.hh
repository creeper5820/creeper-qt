#pragma once

#include <eigen3/Eigen/Dense>
#include <qcolor.h>
#include <qrect.h>

namespace creeper::animate {

template <typename T>
constexpr auto zero() noexcept {
    if constexpr (std::is_arithmetic_v<T>) {
        return T { 0 };
    } else if constexpr (requires { T::Zero(); }) {
        return T::Zero();
    } else {
        static_assert(sizeof(T) == 0, "zero() not implemented for this type");
    }
}

template <typename T>
constexpr auto magnitude(const T& error) noexcept {
    if constexpr (std::is_arithmetic_v<T>) {
        return std::abs(error);
    } else if constexpr (requires { error.norm(); }) {
        return std::abs(error.norm());
    } else {
        static_assert(sizeof(T) == 0, "magnitude() not implemented for this type");
    }
}

}

namespace creeper {

constexpr auto from_color(const QColor& color) -> Eigen::Vector4d {
    return Eigen::Vector4d(color.red(), color.green(), color.blue(), color.alpha());
}
constexpr auto from_vector4(const Eigen::Vector4d& vector) -> QColor {
    return QColor(vector[0], vector[1], vector[2], vector[3]);
}

constexpr auto extract_rect(const QRectF& rect, double w_weight, double h_weight) -> QRectF {
    double rw, rh;
    if (rect.width() * h_weight > rect.height() * w_weight) {
        rh = rect.height();
        rw = rh * w_weight / h_weight;
    } else {
        rw = rect.width();
        rh = rw * h_weight / w_weight;
    }
    const auto center = rect.center();
    return QRectF(center.x() - rw / 2, center.y() - rh / 2, rw, rh);
}

}
