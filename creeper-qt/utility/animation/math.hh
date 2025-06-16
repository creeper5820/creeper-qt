#pragma once

#include <eigen3/Eigen/Dense>

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
inline double calculate_error(const auto& error)
    requires requires { error.norm(); }
{
    return std::abs(error.norm());
}

}
