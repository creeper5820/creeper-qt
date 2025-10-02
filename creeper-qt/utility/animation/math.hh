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

template <typename T>
constexpr auto normalize(const T& error) noexcept {
    if constexpr (std::is_arithmetic_v<T>) {
        return error;
    } else if constexpr (requires { error.norm(); }) {
        return error.norm();
    } else {
        static_assert(sizeof(T) == 0, "magnitude() not implemented for this type");
    }
}

template <typename T>
constexpr auto interpolate(const T& start, const T& end, double t) noexcept -> T {

    const auto clamped_t = std::clamp(t, 0., 1.);

    if constexpr (std::is_arithmetic_v<T>) {
        return static_cast<T>(start + (end - start) * clamped_t);
    } else if constexpr ( //
        requires(const T& a, const T& b, const double f) {
            { a - b } -> std::convertible_to<T>;
            { a * f } -> std::convertible_to<T>;
            { a + b } -> std::convertible_to<T>;
        }) {
        return start + (end - start) * clamped_t;
    } else {
        static_assert(sizeof(T) == 0,
            "interpolate() requires T to be an arithmetic type or define +, -, and scalar "
            "multiplication.");
    }
}

constexpr auto interpolate(const QRectF& start, const QRectF& end, double position) -> QRectF {
    position = qBound(0.0, position, 1.0);
    auto _1  = start.left() + (end.left() - start.left()) * position;
    auto _2  = start.top() + (end.top() - start.top()) * position;
    auto _3  = start.width() + (end.width() - start.width()) * position;
    auto _4  = start.height() + (end.height() - start.height()) * position;
    return { _1, _2, _3, _4 };
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
