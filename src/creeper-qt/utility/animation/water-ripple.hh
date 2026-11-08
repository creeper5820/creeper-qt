#pragma once

#include "creeper-qt/details/drawable/core.hh"
#include "creeper-qt/details/token/ripple/defaults.hh"
#include "creeper-qt/utility/animation/animatable.hh"
#include "creeper-qt/utility/animation/state/accessor.hh"
#include "creeper-qt/utility/animation/transition.hh"

#include <qpainterpath.h>

#include <algorithm>
#include <chrono>
#include <cmath>
#include <memory>
#include <optional>
#include <vector>

namespace creeper {

class WaterRipple {
public:
    explicit WaterRipple(Animatable& animatable)
        : animatable { animatable }
        , ripples { std::make_shared<Ripples>() } { }

    auto press(std::optional<QPointF> origin = std::nullopt) noexcept {
        start(origin);
    }
    auto release() noexcept {
        const auto now = State::Clock::now();
        for (auto iterator = ripples->rbegin(); iterator != ripples->rend(); ++iterator) {
            auto& state = (*iterator)->get_state();
            if (!state.finish_requested) {
                state.finish(now);
                break;
            }
        }
    }
    auto cancel() noexcept -> void {
        const auto now = State::Clock::now();
        for (auto& ripple : *ripples) {
            ripple->get_state().finish(now);
        }
    }
    auto setSize(const QSizeF& size) noexcept -> void {
        this->size = size;
    }
    auto setColor(const QColor& color) noexcept -> void {
        this->color = color;
    }
    auto setClipPath(const QPainterPath& path) noexcept -> void {
        this->clip_path = path;
    }
    auto setBounded(bool bounded) noexcept -> void {
        this->bounded = bounded;
    }
    auto activeCount() const noexcept -> std::size_t {
        return ripples->size();
    }

    friend auto dsl_invoke(drawable::details::draw_t, const WaterRipple& self, QPainter& handler) {
        handler.save();
        {
            handler.setRenderHint(QPainter::Antialiasing);
            handler.setPen(Qt::NoPen);
            if (self.bounded) {
                if (self.clip_path.isEmpty()) {
                    handler.setClipRect(QRectF { QPointF {}, self.size });
                } else {
                    handler.setClipPath(self.clip_path);
                }
            }

            for (const auto& ripple : *self.ripples) {
                const auto& state = ripple->get_state();
                auto color        = self.color;
                color.setAlphaF(color.alphaF() * State::Defaults::kPressedStateLayerOpacity
                    * std::clamp(state.opacity, 0.0, 1.0));
                handler.setBrush(color);
                handler.drawEllipse(state.position, state.radius, state.radius);
            }
        }
        handler.restore();

        std::erase_if(*self.ripples, [](const std::unique_ptr<Ripple>& ripple) {
            if (ripple->get_state().finished) {
                ripple->cancel();
            }
            return ripple->get_state().finished;
        });
    }
    friend auto dsl_invoke(drawable::details::size_t, const WaterRipple& self) {
        return self.size;
    }

private:
    struct State : NormalAccessor {
        using ValueT    = double;
        using Clock     = std::chrono::steady_clock;
        using TimePoint = Clock::time_point;
        using Defaults  = token::ripple::Defaults;

        QPointF origin;
        QPointF center;
        QPointF position;

        double start_radius = 0;
        double end_radius   = 0;
        double radius       = 0;
        double opacity      = 0;

        double value  = 0;
        double target = 1;

        bool finish_requested = false;
        bool finished         = false;

        TimePoint start_time   = Clock::now();
        TimePoint release_time = start_time;

        auto finish(TimePoint now) noexcept -> void {
            if (!finish_requested) {
                finish_requested = true;
                release_time     = now;
            }
        }

        auto update_at(TimePoint now) noexcept -> bool {
            constexpr auto kEnterDuration =
                std::chrono::duration<double> { Defaults::kRadiusDuration }.count();
            constexpr auto kFadeInDuration =
                std::chrono::duration<double> { Defaults::kFadeInDuration }.count();
            constexpr auto kFadeOutDuration =
                std::chrono::duration<double> { Defaults::kFadeOutDuration }.count();

            const auto elapsed = std::chrono::duration<double> { now - start_time }.count();

            const auto radius_progress = std::clamp(elapsed / kEnterDuration, 0.0, 1.0);
            const auto center_progress = std::clamp(elapsed / kEnterDuration, 0.0, 1.0);

            value  = radius_progress;
            radius = std::lerp(
                start_radius, end_radius, Defaults::kFastOutSlowInEasing.solve(radius_progress));
            position = origin + (center - origin) * center_progress;

            if (finish_requested) {
                const auto fade_begin =
                    std::max(start_time + Defaults::kRadiusDuration, release_time);
                const auto fade_elapsed =
                    std::chrono::duration<double> { now - fade_begin }.count();
                if (fade_elapsed >= 0) {
                    const auto fade_progress =
                        std::clamp(fade_elapsed / kFadeOutDuration, 0.0, 1.0);
                    opacity  = 1 - Defaults::kEasingLinearCubicBezier.solve(fade_progress);
                    finished = fade_progress >= 1;
                } else {
                    opacity = 1;
                }
            } else {
                opacity = Defaults::kEasingLinearCubicBezier.solve(
                    std::clamp(elapsed / kFadeInDuration, 0.0, 1.0));
            }
            return !finished;
        }

        auto update() noexcept -> bool {
            return update_at(Clock::now());
        }
    };
    using Ripple  = TransitionValue<State>;
    using Ripples = std::vector<std::unique_ptr<Ripple>>;

    auto start(std::optional<QPointF> origin) noexcept -> void {
        const auto state = std::make_shared<State>();
        const auto bounds =
            clip_path.isEmpty() ? QRectF { QPointF {}, size } : clip_path.boundingRect();
        const auto center = bounds.center();

        state->origin   = origin.value_or(center);
        state->center   = center;
        state->position = state->origin;
        state->start_radius =
            std::max(size.width(), size.height()) * State::Defaults::kStartRadiusScale;
        state->end_radius = std::hypot(size.width(), size.height()) / 2;
        if (bounded) state->end_radius += State::Defaults::kBoundedRippleExtraRadius.value;
        state->radius = state->start_radius;

        auto ripple = make_transition(animatable, state);
        ripple->transition_to(1.0);

        ripples->push_back(std::move(ripple));
    }

    Animatable& animatable;
    std::shared_ptr<Ripples> ripples;

    QSizeF size;
    QColor color = Qt::black;
    QPainterPath clip_path;
    bool bounded = true;
};

}
