#pragma once

#include "creeper-qt/utility/animation/core.hh"

#include <algorithm>
#include <qdebug.h>
#include <qevent.h>
#include <qpaintdevice.h>
#include <qpainter.h>
#include <qpainterpath.h>

namespace creeper::util::animation {

struct WaterRippleData {
    QPointF origin;
    double radius  = 0.0;
    double opacity = 1.0;
    bool finished  = false;
};
struct WaterRippleAnimation  : IAnimation {
    explicit WaterRippleAnimation(
        const std::shared_ptr<WaterRippleData>& data, double speed, double max_distance, double hz)
        : data(data)
        , speed(speed)
        , max_distance(max_distance) { }

    bool update() override {
        data->radius   = data->radius + speed;
        data->finished = data->radius > max_distance;
        data->opacity  = 1.0 - data->radius / max_distance;
        return data->finished;
    }

    std::shared_ptr<WaterRippleData> data;
    double speed;
    double max_distance;
};
class WaterRippleRenderer {
public:
    explicit WaterRippleRenderer(AnimationCore& core, double speed, double hz)
        : animation_core { core }
        , speed { speed }
        , hz { hz } { }

    void clicked(QPointF origin, double max_distance) noexcept {
        auto data = std::make_shared<WaterRippleData>(origin);
        animation_core.append(
            std::make_unique<WaterRippleAnimation>(data, speed, max_distance, hz));
        water_ripples.push_back(std::move(data));
    }

    auto renderer(const QPainterPath& clip_path, const QColor& water_color) noexcept {
        return [this, &clip_path, &water_color](QPainter& painter) {
            const auto [_0, _1] = std::ranges::remove_if(water_ripples, [&](const auto& data) {
                painter.setRenderHint(QPainter::Antialiasing);
                painter.setClipPath(clip_path);
                painter.setOpacity(data->opacity);
                painter.setPen(Qt::NoPen);
                painter.setBrush(water_color);
                painter.drawEllipse(data->origin, data->radius, data->radius);
                painter.setOpacity(1.0);
                return data->finished;
            });
            water_ripples.erase(_0, _1);
        };
    }

private:
    std::vector<std::shared_ptr<WaterRippleData>> water_ripples;
    AnimationCore& animation_core;
    double speed, hz;
};

}
