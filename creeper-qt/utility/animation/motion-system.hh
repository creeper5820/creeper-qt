#pragma once

#include "creeper-qt/utility/animation/core.hh"
#include "creeper-qt/utility/animation/math.hh"
#include <eigen3/Eigen/Dense>

namespace creeper::util::animation {

constexpr auto kErrorThreshold = 1e-2;

template <typename T> struct FinitePidTracker : public IAnimation {
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
        const auto error = T { target - *current };

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

    /// Note:
    ///     牢记 Eigen 变量非 POD，默认不初始化
    ///     必须要使用 VectorX::Zero() 来初始化
    T integral   = zero<T>();
    T last_error = zero<T>();
};

template <typename T> struct FiniteSringTracker final : IAnimation {
    std::shared_ptr<T> current;
    std::shared_ptr<bool> stop_token;
    T target;
    double k, d, dt;
    T velocity = zero<T>();

    FiniteSringTracker(const std::shared_ptr<T>& current, const T& target,
        const std::shared_ptr<bool>& stop_token, double k, double d, double hz)
        : current(current)
        , target(target)
        , stop_token(stop_token)
        , k(k)
        , d(d)
        , dt(1.0 / hz) { }

    // ~FiniteSringTracker() { qDebug() << "FiniteSringTracker Destroy"; }

    bool update() {
        if (!current || !stop_token) return true;
        const auto error = T { *current - target };

        const auto a_force   = T { -k * error };
        const auto a_damping = T { -d * velocity };
        const auto a_total   = T { a_force + a_damping };

        velocity = velocity + a_total * dt;
        *current = *current + velocity * dt;

        return (calculate_error(error) < kErrorThreshold && std::abs(velocity) < kErrorThreshold)
            || *stop_token;
    }
};

struct MotionScheme { };

}
