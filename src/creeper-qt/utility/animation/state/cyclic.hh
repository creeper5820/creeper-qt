#pragma once
#include "creeper-qt/utility/animation/math.hh"
#include "creeper-qt/utility/animation/state/accessor.hh"

#include <chrono>
#include <cmath>
#include <concepts>

namespace creeper {

/// 永续循环相位状态：value 以 config.speed（周期/秒）匀速前进，
/// 到达 config.period 后回绕，永不收敛（update 恒返回 true）。
///
/// 启停通过 TransitionValue 控制：
/// - transition_to：推入永续任务，动画开始
/// - snap_to：使运行令牌失效，任务在下一 tick 被移除，动画停止
template <std::floating_point T = double>
struct CyclicState : public NormalAccessor {
    using ValueT    = T;
    using Clock     = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;

    T value  = animate::zero<T>();
    T target = animate::zero<T>();

    struct {
        double speed  = 1.0;
        double period = 1.0;
    } config;

    struct {
        TimePoint last_timestamp = Clock::now();
    } details;

    auto update() noexcept -> bool {
        const auto now      = Clock::now();
        const auto duration = now - details.last_timestamp;
        const auto dt       = std::chrono::duration<double>(duration).count();

        details.last_timestamp = now;

        value += static_cast<T>(config.speed * dt);
        value = std::fmod(value, static_cast<T>(config.period));
        if (value < T { 0 }) value += static_cast<T>(config.period);

        return true;
    }
};

}
