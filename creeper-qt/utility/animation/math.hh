#pragma once

#include <eigen3/Eigen/Dense>
#include <qcolor.h>

namespace creeper::util::animation {

template <typename T> inline T update_pid_using_target(T value, T target, double kp = 1) {
    return static_cast<T>(value) + kp * (static_cast<T>(target) - static_cast<T>(value));
}

template <typename T> inline T update_pid_using_error(T value, T error, double kp = 1) {
    return static_cast<T>(value) + kp * static_cast<T>(error);
}

template <typename T> inline T zero();

template <> inline double zero() { return 0.; }
inline double calculate_error(double error) { return std::abs(error); }

template <> inline Eigen::Vector2d zero() { return Eigen::Vector2d::Zero(); }
inline double calculate_error(const Eigen::Vector2d& error) { return std::abs(error.norm()); }

template <> inline Eigen::Vector4d zero() { return Eigen::Vector4d::Zero(); }
inline double calculate_error(const Eigen::Vector4d& error) { return std::abs(error.norm()); }

}

namespace creeper {

inline Eigen::Vector4d from_color(const QColor& color) {
    return Eigen::Vector4d(color.red(), color.green(), color.blue(), color.alpha());
}
inline QColor from_vector4(const Eigen::Vector4d& vector) {
    return QColor(vector[0], vector[1], vector[2], vector[3]);
}

}
