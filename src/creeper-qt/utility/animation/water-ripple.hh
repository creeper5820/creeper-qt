#pragma once

#include "creeper-qt/details/token/ripple/values.hh"
#include "creeper-qt/utility/animation/state/accessor.hh"
#include "creeper-qt/utility/animation/transition.hh"
#include "creeper-qt/utility/math/cubic-bezier.hh"

#include <QColor>
#include <QPainter>
#include <QPainterPath>
#include <QPointF>
#include <QSizeF>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <memory>

namespace creeper {

using Ms = std::chrono::milliseconds;

struct WaterRippleState : public NormalAccessor {
    using ValueT    = double;
    using Clock     = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;

    Ms fade_in_duration;
    Ms radius_duration;
    Ms fade_out_duration;
    CubicBezierSolution radius_easing;

    QPointF origin;
    QPointF center;
    QPointF position;

    double start_radius = 0.0;
    double value        = 0.0;
    double target       = 0.0;
    double opacity      = 0.0;
    bool finished       = false;
    bool released       = false;

    TimePoint start_time   = Clock::now();
    TimePoint release_time = Clock::now();

    auto set_target(double new_target) noexcept -> void {
        target       = new_target;
        value        = start_radius;
        position     = origin;
        opacity      = 0.0;
        finished     = false;
        released     = false;
        start_time   = Clock::now();
        release_time = start_time;
    }

    auto release() noexcept -> void {
        released     = true;
        release_time = Clock::now();
    }

    auto update() noexcept -> bool {
        const auto now     = Clock::now();
        const auto elapsed = std::chrono::duration<double>(now - start_time).count();

        const auto fade_in  = seconds(fade_in_duration);
        const auto grow     = seconds(radius_duration);
        const auto fade_out = seconds(fade_out_duration);

        // 半径用 FastOutSlowIn，圆心用 Linear，两者同步推进
        const auto grow_progress = progress(elapsed, grow);
        value    = std::lerp(start_radius, target, radius_easing.solve(grow_progress));
        position = origin + (center - origin) * grow_progress;

        // 淡入与扩张都结束后才允许淡出；未淡入完成就松开时，直接跳到最终透明度
        const auto fade_in_end = std::max(fade_in, grow);
        if (!released) {
            opacity = progress(elapsed, fade_in);
        } else if (elapsed < fade_in_end) {
            opacity = 1.0;
        } else {
            const auto released_offset =
                std::chrono::duration<double>(release_time - start_time).count();
            const auto fade_begin = std::max(fade_in_end, released_offset);

            const auto fade_out_progress = progress(elapsed - fade_begin, fade_out);
            opacity                      = 1.0 - fade_out_progress;
            finished                     = fade_out_progress >= 1.0;
        }

        return !finished;
    }

private:
    static auto seconds(Ms duration) noexcept -> double {
        return std::chrono::duration<double>(duration).count();
    }

    static auto progress(double elapsed, double duration) noexcept -> double {
        if (duration <= 0.0) return 1.0;
        return std::clamp(elapsed / duration, 0.0, 1.0);
    }
};

class WaterRippleRenderer {
public:
    explicit WaterRippleRenderer(Animatable& core)
        : animatable { core } { }

    auto set_fade_in_duration(Ms duration) noexcept -> void { fade_in_duration = duration; }
    auto set_radius_duration(Ms duration) noexcept -> void { radius_duration = duration; }
    auto set_fade_out_duration(Ms duration) noexcept -> void { fade_out_duration = duration; }
    auto set_radius_easing(CubicBezierSolution easing) noexcept -> void { radius_easing = easing; }
    auto set_start_radius_factor(double factor) noexcept -> void { start_radius_factor = factor; }
    auto set_extra_radius(Dp radius) noexcept -> void { extra_radius = radius; }

    /// @brief 按下：开始扩张与淡入；若已有活跃 ripple 则先中断
    auto press(const QPointF& origin, const QSizeF& size) noexcept -> void {
        if (active) active->cancel();

        auto state               = std::make_shared<WaterRippleState>();
        state->fade_in_duration  = fade_in_duration;
        state->radius_duration   = radius_duration;
        state->fade_out_duration = fade_out_duration;
        state->radius_easing     = radius_easing;

        state->origin       = origin;
        state->center       = QPointF { size.width() / 2.0, size.height() / 2.0 };
        state->start_radius = std::max(size.width(), size.height()) * start_radius_factor;

        const auto end_radius = std::hypot(size.width(), size.height()) / 2.0 + extra_radius.value;

        active = make_transition(animatable, state);
        active->transition_to(end_radius);
    }

    /// @brief 松开：当前活跃 ripple 开始淡出
    auto release() noexcept -> void {
        if (!active) return;
        auto& state = active->get_state();
        if (!state.finished) state.release();
    }

    auto renderer(const QPainterPath& clip_path, const QColor& water_color) noexcept {
        return [&, this](QPainter& painter) {
            if (!active) return;

            const auto& state = active->get_state();

            painter.setRenderHint(QPainter::Antialiasing);
            painter.setClipPath(clip_path);
            painter.setOpacity(std::clamp(state.opacity, 0.0, 1.0));
            painter.setPen(Qt::NoPen);
            painter.setBrush(water_color);
            painter.drawEllipse(state.position, state.value, state.value);
            painter.setOpacity(1.0);

            if (state.finished) active.reset();
        };
    }

private:
    Ms fade_in_duration  = token::ripple::kFadeInDuration;
    Ms radius_duration   = token::ripple::kRadiusDuration;
    Ms fade_out_duration = token::ripple::kFadeOutDuration;

    CubicBezierSolution radius_easing = token::ripple::kEasingLegacyCubicBezier;

    double start_radius_factor = token::ripple::kStartRadiusFactor;
    Dp extra_radius            = token::ripple::kExtraRadius;

    std::unique_ptr<TransitionValue<WaterRippleState>> active;
    Animatable& animatable;
};

}
