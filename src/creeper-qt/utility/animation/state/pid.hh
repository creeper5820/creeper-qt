#pragma once
#include "creeper-qt/utility/animation/math.hh"
#include "creeper-qt/utility/animation/state/accessor.hh"

#include <chrono>

namespace creeper {

template <typename T>
struct PidState : public NormalAccessor {
    using ValueT = T;

    using Clock     = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;

    T value  = animate::zero<T>();
    T target = animate::zero<T>();

    struct {
        double kp      = 1.0;
        double ki      = 0.0;
        double kd      = 0.1;
        double epsilon = 1e-3;
    } config;

    struct {
        T integral_error = animate::zero<T>();
        T last_error     = animate::zero<T>();
        TimePoint last_timestamp;
    } details;

    auto set_target(T new_target) noexcept -> void {
        target = new_target;

        const auto current_time = Clock::now();

        using namespace std::chrono_literals;
        const auto threshold = 16ms;

        const auto elapsed_time = current_time - details.last_timestamp;

        if (elapsed_time > threshold) {
            details.last_error     = target - value;
            details.last_timestamp = current_time;
        }
    }

    auto update() noexcept -> bool {

        const auto kp = config.kp;
        const auto ki = config.ki;
        const auto kd = config.kd;

        const auto now      = Clock::now();
        const auto duration = now - details.last_timestamp;

        const auto dt = std::chrono::duration<double>(duration).count();

        if (dt <= 0.0) {
            details.last_timestamp = now;
            return animate::magnitude(target - value) > config.epsilon;
        }

        const auto current_error = target - value;

        if (animate::magnitude(current_error) <= config.epsilon
            && animate::magnitude(details.last_error) <= config.epsilon) {
            value                  = target;
            details.integral_error = animate::zero<T>();
            details.last_error     = animate::zero<T>();
            details.last_timestamp = now;
            return false;
        }

        const auto proportional_term = kp * current_error;

        details.integral_error += current_error * dt;
        const auto integral_term = ki * details.integral_error;

        const auto derivative_error = (current_error - details.last_error) / dt;
        const auto derivative_term  = kd * derivative_error;

        const auto output = proportional_term + integral_term + derivative_term;

        value += output * dt;

        details.last_error     = current_error;
        details.last_timestamp = now;

        return true;
    }
};

}
