#pragma once
#include "creeper-qt/utility/animation/math.hh"
#include "creeper-qt/utility/animation/state/accessor.hh"
#include "creeper-qt/utility/math/cubic-bezier.hh"

#include <algorithm>
#include <chrono>

namespace creeper {

/// @brief 定时缓动状态：在固定时长内按 cubic-bezier 从起点插值到目标
///
/// 对应 MD3 的 duration / easing token；`set_target` 时重置起点与计时。
template <typename T>
struct TweenState : public NormalAccessor {
    using ValueT = T;

    using Clock     = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;

    T value  = animate::zero<T>();
    T target = animate::zero<T>();

    struct {
        std::chrono::duration<double> duration = std::chrono::milliseconds { 300 };
        CubicBezierSolution easing             = kStandardEase;
    } config;

    struct {
        T origin        = animate::zero<T>();
        TimePoint start = Clock::now();
    } details;

    auto set_target(T new_target) noexcept -> void {
        const auto epsilon = 1e-6;
        if (animate::magnitude(new_target - target) <= epsilon) {
            target = std::move(new_target);
            return;
        }

        details.origin = value;
        target         = std::move(new_target);
        details.start  = Clock::now();
    }

    auto update() noexcept -> bool {
        const auto now     = Clock::now();
        const auto elapsed = std::chrono::duration<double>(now - details.start);

        const auto progress = (config.duration > std::chrono::duration<double>::zero())
            ? std::clamp(elapsed / config.duration, 0.0, 1.0)
            : 1.0;
        const auto eased    = config.easing.solve(progress);

        value = animate::interpolate(details.origin, target, eased);
        return progress < 1.0;
    }
};

}
