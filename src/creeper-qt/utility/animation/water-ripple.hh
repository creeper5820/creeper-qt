#pragma once

#include "creeper-qt/utility/animation/state/accessor.hh"
#include "creeper-qt/utility/animation/transition.hh"
#include "creeper-qt/utility/math/cubic-bezier.hh"

#include <QColor>
#include <QPainter>
#include <QPainterPath>
#include <QPointF>

#include <algorithm>
#include <chrono>
#include <memory>

namespace creeper {

struct WaterRippleState : public NormalAccessor {
    using ValueT    = double;
    using Clock     = std::chrono::steady_clock;
    using TimePoint = Clock::time_point;

    /// MD3 ripple 时序（秒）
    static constexpr auto kGrowDuration    = 0.450;
    static constexpr auto kFadeInDuration  = 0.105;
    static constexpr auto kFadeOutDuration = 0.375;
    static constexpr auto kMinimumPress    = 0.225;

    QPointF origin;
    double value   = 0.0;
    double target  = 0.0;
    double opacity = 0.0;
    bool finished  = false;
    bool released  = false;

    TimePoint start_time   = Clock::now();
    TimePoint release_time = Clock::now();

    auto set_target(double new_target) noexcept -> void {
        target       = new_target;
        value        = 0.0;
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

        const auto grow_progress = std::clamp(elapsed / kGrowDuration, 0.0, 1.0);
        value                    = target * kStandardEase.solve(grow_progress);

        const auto fade_in_progress = std::clamp(elapsed / kFadeInDuration, 0.0, 1.0);
        opacity                     = fade_in_progress;

        if (released) {
            const auto released_offset =
                std::chrono::duration<double>(release_time - start_time).count();
            const auto fade_begin = std::max(kMinimumPress, released_offset);

            if (elapsed >= fade_begin) {
                const auto fade_out_progress =
                    std::clamp((elapsed - fade_begin) / kFadeOutDuration, 0.0, 1.0);
                opacity  = 1.0 - fade_out_progress;
                finished = fade_out_progress >= 1.0;
            } else {
                opacity = 1.0;
            }
        }

        return !finished;
    }
};

class WaterRippleRenderer {
public:
    explicit WaterRippleRenderer(Animatable& core)
        : animatable { core } { }

    /// @brief 按下：开始扩张与淡入；若已有活跃 ripple 则先中断
    auto press(const QPointF& origin, double max_distance) noexcept -> void {
        if (active) active->cancel();

        auto state    = std::make_shared<WaterRippleState>();
        state->origin = origin;

        active = make_transition(animatable, state);
        active->transition_to(max_distance);
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
            painter.drawEllipse(state.origin, state.value, state.value);
            painter.setOpacity(1.0);

            if (state.finished) active.reset();
        };
    }

private:
    std::unique_ptr<TransitionValue<WaterRippleState>> active;
    Animatable& animatable;
};

}
