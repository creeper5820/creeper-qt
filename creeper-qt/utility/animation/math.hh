#pragma once

namespace creeper::util::animation {

template <typename T> inline T update_pid_using_target(T value, T target, double kp = 1) {
    return static_cast<T>(value) + kp * (static_cast<T>(target) - static_cast<T>(value));
}

template <typename T> inline T update_pid_using_error(T value, T error, double kp = 1) {
    return static_cast<T>(value) + kp * static_cast<T>(error);
}

}
