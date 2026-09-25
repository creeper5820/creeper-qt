#pragma once
#include "creeper-qt/utility/animation/math.hh"
#include "creeper-qt/utility/animation/state/accessor.hh"

#include <algorithm>
#include <chrono>
#include <cmath>

namespace creeper {

template <typename T>
struct SpringState : public NormalAccessor {
    using ValueT    = T;
    using Clock     = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;

    T value { animate::zero<T>() };
    T target { animate::zero<T>() };

    T velocity = animate::zero<T>();

    TimePoint last_timestamp = Clock::now();

    struct {
        double k       = 1.0;
        double d       = 0.1;
        double epsilon = 1e-1;
        // 显式欧拉积分的最大步长，超出时拆分为子步，保证任意帧率下收敛
        double max_dt = 1.0 / 60.0;
    } config;

    auto set_target(T new_target) noexcept -> void {
        // 仅在目标变化时重置速度
        if (animate::magnitude(new_target - target) <= config.epsilon) {
            target = new_target;
            return;
        }

        target = new_target;
        velocity = animate::zero<T>();
        last_timestamp = Clock::now();
    }

    auto update() noexcept -> bool {
        const auto now      = Clock::now();
        const auto duration = now - last_timestamp;
        const double dt     = std::chrono::duration<double>(duration).count();

        if (dt <= 0.0) {
            last_timestamp = now;
            return std::abs(animate::magnitude(target - value)) > config.epsilon;
        }

        // 将实际帧间隔拆分为若干固定大小的子步，避免显式欧拉在大 dt 下失稳
        const auto substep_count = std::max(1, static_cast<int>(std::ceil(dt / config.max_dt)));
        const auto sub_dt        = dt / substep_count;

        for (int i = 0; i < substep_count; ++i) {
            const auto error     = value - target;
            const auto a_force   = -config.k * error;
            const auto a_damping = -config.d * velocity;
            const auto a_total   = a_force + a_damping;

            velocity += a_total * sub_dt;
            value += velocity * sub_dt;
        }

        last_timestamp = now;

        const bool done = animate::magnitude(value - target) < config.epsilon &&
            animate::magnitude(velocity) < config.epsilon;

        if (done) velocity = animate::zero<T>();
        return !done;
    }
};
}
