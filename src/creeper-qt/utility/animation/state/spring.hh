#pragma once
#include "creeper-qt/utility/animation/math.hh"
#include "creeper-qt/utility/animation/state/accessor.hh"

#include <chrono>

namespace creeper {

template <typename T>
struct SpringState : public NormalAccessor {
    using ValueT    = T;
    using Clock     = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;

    T value;
    T target;

    T velocity = animate::zero<T>();

    TimePoint last_timestamp = Clock::now();

    struct {
        double k       = 1.0;
        double d       = 0.1;
        double epsilon = 1e-1;
    } config;

    auto set_target(T new_target) noexcept -> void {
        target = new_target;

        const auto current_time = Clock::now();

        using namespace std::chrono_literals;
        const auto threshold = 16ms;

        const auto elapsed_time = current_time - last_timestamp;

        if (elapsed_time > threshold) {
            const auto error = target - value;
            velocity         = animate::zero<T>();
            last_timestamp   = current_time;
        }
    }

    auto update() noexcept -> bool {
        const auto now      = Clock::now();
        const auto duration = now - last_timestamp;
        const double dt     = std::chrono::duration<double>(duration).count();

        if (dt <= 0.0) {
            last_timestamp = now;
            return std::abs(animate::magnitude(target - value)) > config.epsilon;
        }

        const auto error     = value - target;
        const auto a_force   = -config.k * error;
        const auto a_damping = -config.d * velocity;
        const auto a_total   = a_force + a_damping;

        velocity += a_total * dt;
        value += velocity * dt;

        last_timestamp = now;

        const bool done =
            animate::magnitude(error) < config.epsilon && std::abs(velocity) < config.epsilon;

        if (done) velocity = animate::zero<T>();
        return !done;
    }
};
}
