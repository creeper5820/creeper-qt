#pragma once

#include "creeper-qt/utility/animation/transition.hh"

#include <QColor>
#include <QPainter>
#include <QPainterPath>
#include <QPointF>
#include <memory>
#include <vector>

namespace creeper {

struct WaterRippleState : public NormalAccessor {
    using ValueT = double;

    QPointF origin;
    double value  = 0.0;
    double target = 0.0;
    double speed  = 1.0;

    auto update() noexcept -> bool {
        value += speed;
        return value < target;
    }
};

class WaterRippleRenderer {
public:
    explicit WaterRippleRenderer(Animatable& core, double speed)
        : animatable { core }
        , speed { speed } { }

    auto clicked(const QPointF& origin, double max_distance) noexcept -> void {
        auto state    = std::make_shared<WaterRippleState>();
        state->origin = origin;
        state->speed  = speed;
        state->target = max_distance;

        auto ripple = make_transition(animatable, state);
        ripple->transition_to(max_distance);
        ripples.push_back(std::move(ripple));
    }

    auto renderer(const QPainterPath& clip_path, const QColor& water_color) noexcept {
        return [&, this](QPainter& painter) {
            std::erase_if(ripples, [&](const auto& ripple) {
                const auto& state  = ripple->get_state();
                const auto opacity = 1.0 - state.value / state.target;

                painter.setRenderHint(QPainter::Antialiasing);
                painter.setClipPath(clip_path);
                painter.setOpacity(opacity);
                painter.setPen(Qt::NoPen);
                painter.setBrush(water_color);
                painter.drawEllipse(state.origin, state.value, state.value);
                painter.setOpacity(1.0);

                return state.value >= state.target;
            });
        };
    }

private:
    std::vector<std::unique_ptr<TransitionValue<WaterRippleState>>> ripples;
    Animatable& animatable;
    double speed;
};

}
