#pragma once

#include <algorithm>
#include <cmath>

namespace creeper {

/// @brief 三次贝塞尔缓动解，对应 CSS cubic-bezier(x1, y1, x2, y2)
/// @note 曲线固定经过 (0,0) 与 (1,1)，与 CSS / MD3 motion token 一致
struct CubicBezierSolution {
    double x1 { 0 };
    double y1 { 0 };
    double x2 { 0 };
    double y2 { 0 };

    static constexpr auto kEpsilon = 1e-7;

    /// @param progress 归一化进度 x，夹取到 [0, 1]
    /// @return 缓动后的 y
    auto solve(double progress) const noexcept -> double {
        const auto x = std::clamp(progress, 0.0, 1.0);
        if (x <= 0.0) return 0.0;
        if (x >= 1.0) return 1.0;

        // Newton 迭代求解 x(t) = x
        auto t = x;
        for (auto i = 0; i < 8; ++i) {
            const auto error = sample(t, x1, x2) - x;
            if (std::abs(error) < kEpsilon) return sample(t, y1, y2);

            const auto derivative = slope(t, x1, x2);
            if (std::abs(derivative) < kEpsilon) break;

            t = std::clamp(t - error / derivative, 0.0, 1.0);
        }

        // 二分兜底
        auto lower = double { 0 };
        auto upper = double { 1 };
        t          = x;
        while (upper - lower > kEpsilon) {
            if (sample(t, x1, x2) < x) lower = t;
            else upper = t;
            t = (lower + upper) / 2;
        }
        return sample(t, y1, y2);
    }

private:
    static constexpr auto sample(double t, double p1, double p2) noexcept -> double {
        const auto u = 1.0 - t;
        return 3.0 * u * u * t * p1 + 3.0 * u * t * t * p2 + t * t * t;
    }
    static constexpr auto slope(double t, double p1, double p2) noexcept -> double {
        const auto u = 1.0 - t;
        return 3.0 * u * u * p1 + 6.0 * u * t * (p2 - p1) + 3.0 * t * t * (1.0 - p2);
    }
};

/// MD3 standard easing
inline constexpr auto kStandardEase = CubicBezierSolution { 0.2, 0.0, 0.0, 1.0 };

}
