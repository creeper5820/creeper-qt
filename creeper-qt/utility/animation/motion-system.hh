#pragma once

#include <eigen3/Eigen/Dense>

#include "creeper-qt/utility/animation/core.hh"
#include "creeper-qt/utility/animation/math.hh"

namespace creeper::animate {

constexpr auto kErrorThreshold = 1e-1;

template <typename T>
struct FinitePidTracker : public IAnimation {
    explicit FinitePidTracker(const std::shared_ptr<T>& current, const T& target,
        const std::shared_ptr<bool>& stop_token, double kp, double ki, double kd, double hz,
        double error_threshold = kErrorThreshold)
        : current(current)
        , stop_token(stop_token)
        , target(target)
        , kp(kp)
        , ki(ki)
        , kd(kd)
        , dt(1.0 / hz)
        , error_threshold(error_threshold) { }

    bool update() {
        if (!current || !stop_token) return true;
        const auto error = T { target - *current };

        integral     = integral + error * dt;
        T derivative = (error - last_error) / dt;
        T output     = kp * error + ki * integral + kd * derivative;
        last_error   = error;

        *current = *current + T { output * dt };

        return magnitude(error) < error_threshold || *stop_token;
    }

    std::shared_ptr<T> current;
    std::shared_ptr<bool> stop_token;
    T target;
    double kp, ki, kd, dt;
    double error_threshold;

    /// Note:
    ///     牢记 Eigen 变量非 POD，默认不初始化
    ///     必须要使用 VectorX::Zero() 来初始化
    T integral   = zero<T>();
    T last_error = zero<T>();
};

template <typename T>
struct FiniteSringTracker : IAnimation {

    FiniteSringTracker(const std::shared_ptr<T>& current, const T& target,
        const std::shared_ptr<bool>& stop_token, double k, double d, double hz,
        double error_threshold = kErrorThreshold)
        : current(current)
        , stop_token(stop_token)
        , target(target)
        , k(k)
        , d(d)
        , dt(1.0 / hz)
        , error_threshold(error_threshold) { }

    bool update() {
        if (!current || !stop_token) return true;
        const auto error = T { *current - target };

        const auto a_force   = T { -k * error };
        const auto a_damping = T { -d * velocity };
        const auto a_total   = T { a_force + a_damping };

        velocity = velocity + a_total * dt;
        *current = *current + velocity * dt;

        return (magnitude(error) < error_threshold && std::abs(velocity) < error_threshold)
            || *stop_token;
    }

    std::shared_ptr<T> current;
    std::shared_ptr<bool> stop_token;
    T target;
    double k, d, dt;
    double error_threshold;

    T velocity = zero<T>();
};

struct MotionScheme { };

}
