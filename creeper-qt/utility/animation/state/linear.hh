#pragma once
#include "creeper-qt/utility/animation/math.hh"
#include "creeper-qt/utility/animation/state/accessor.hh"

#include <chrono>

namespace creeper {

template <typename T>
struct LinearState : public NormalAccessor {
    using ValueT = T;

    using Clock     = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;

    T value  = animate::zero<T>();
    T target = animate::zero<T>();

    struct {
        double speed   = 1.0;
        double epsilon = 1e-2;
    } config;

    struct {
        TimePoint last_timestamp;
    } details;

    auto set_target(T new_target) noexcept -> void {
        target = new_target;

        const auto current_time = Clock::now();
        using namespace std::chrono_literals;
        const auto threshold = 16ms;

        const auto elapsed_time = current_time - details.last_timestamp;

        if (elapsed_time > threshold) {
            details.last_timestamp = current_time;
        }
    }

    auto update() noexcept -> bool {
        const auto now      = Clock::now();
        const auto duration = now - details.last_timestamp;
        const auto dt       = std::chrono::duration<double>(duration).count();

        if (dt <= 0.0) {
            details.last_timestamp = now;
            return animate::magnitude(target - value) > config.epsilon;
        }

        const auto delta = target - value;
        const auto dist  = animate::magnitude(delta);

        if (dist <= config.epsilon) {
            value                  = target;
            details.last_timestamp = now;
            return false;
        }

        const auto direction = animate::normalize(delta);
        const auto step      = config.speed * dt;

        if (step >= dist) {
            value = target;
        } else {
            value += direction * step;
        }

        details.last_timestamp = now;
        return true;
    }
};

}
