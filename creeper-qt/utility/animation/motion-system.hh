#pragma once

#include "utility/animation/core.hh"
#include "utility/animation/math.hh"
#include <eigen3/Eigen/Dense>
#include <qeasingcurve.h>

namespace creeper::util::animation {

constexpr auto kErrorThreshold = 1e-2;

template <typename T> struct FinitePidTracker final : IAnimation {
    explicit FinitePidTracker(const std::shared_ptr<T>& current, const T& target,
        const std::shared_ptr<bool>& stop_token, double kp, double ki, double kd, double hz)
        : current(current)
        , target(target)
        , stop_token(stop_token)
        , kp(kp)
        , ki(ki)
        , kd(kd)
        , dt(1.0 / hz) { }

    bool update() {
        if (!current || !stop_token) return true;
        const T error = target - *current;

        integral     = integral + error * dt;
        T derivative = (error - last_error) / dt;
        T output     = kp * error + ki * integral + kd * derivative;
        last_error   = error;

        *current = *current + T { output * dt };

        return calculate_error(error) < kErrorThreshold || *stop_token;
    }

    std::shared_ptr<T> current;
    std::shared_ptr<bool> stop_token;
    T target;
    double kp, ki, kd, dt;

    T integral   = T {};
    T last_error = T {};
};

template <typename T> struct InfinitePidTracker : IAnimation {
    explicit InfinitePidTracker(const std::shared_ptr<T>& current, const std::shared_ptr<T>& target,
        const std::shared_ptr<bool>& stop_token, double kp, double ki, double kd, double hz)
        : current(current)
        , target(target)
        , stop_token(stop_token)
        , kp(kp)
        , ki(ki)
        , kd(kd)
        , dt(1.0 / hz) { }

    bool update() {
        if (!current || !target || !stop_token) return true;

        const T error = *target - *current;
        if (calculate_error(error) < kErrorThreshold) return false;

        integral     = integral + error * dt;
        T derivative = (error - last_error) / dt;
        T output     = kp * error + ki * integral + kd * derivative;
        last_error   = error;

        *current = *current + T { output * dt };

        return *stop_token;
    }

    std::shared_ptr<T> current, target;
    std::shared_ptr<bool> stop_token;
    double kp, ki, kd, dt;

    T integral   = zero<T>();
    T last_error = zero<T>();
};

template <typename T> struct InfiniteSringTracker final : IAnimation {
    std::shared_ptr<T> current, target;
    std::shared_ptr<bool> stop_token;
    double k, d, dt;
    T velocity = zero<T>();

    InfiniteSringTracker(const std::shared_ptr<T>& current, const std::shared_ptr<T>& target,
        const std::shared_ptr<bool>& stop_token, double k, double d, double hz)
        : current(current)
        , target(target)
        , stop_token(stop_token)
        , k(k)
        , d(d)
        , dt(1.0 / hz) { }

    bool update() {
        if (!current || !target || !stop_token) return true;
        if (calculate_error(T { *current - *target }) < kErrorThreshold) return false;

        const auto a_force   = T { -k * T { *current - *target } };
        const auto a_damping = T { -d * velocity };
        const auto a_total   = T { a_force + a_damping };

        velocity = velocity + a_total * dt;
        *current = *current + velocity * dt;

        return *stop_token;
    }
};

struct MotionScheme { };
}
