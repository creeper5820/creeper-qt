#pragma once

#include "utility/animation/core.hh"
#include "utility/painter/helper.hh"
#include <qevent.h>
#include <qpaintdevice.h>
#include <qpainter.h>
#include <qpainterpath.h>

namespace creeper::util::animation {

struct WaterRipple final : IAnimation {
public:
    using Renderer = std::function<void(QPainter&)>;

    WaterRipple(const QColor& color, const QPainterPath& path, const QPointF& origin, double speed,
        double max_distance, const std::function<bool(const Renderer&)>& callback)
        : color(color)
        , path(path)
        , origin(origin)
        , speed(speed)
        , max_distance(max_distance)
        , callback(callback) { }

    bool update() override {
        const auto is_finish = (distance += speed) > max_distance;
        const auto renderer  = is_finish ? Renderer { [](auto&) {} } : [this](QPainter& painter) {
            PainterHelper { painter }
                .set_clip_path(path)
                .set_opacity(1. - distance / max_distance)
                .ellipse(color, Qt::transparent, 0, origin, distance, distance)
                .done();
        };
        const auto is_exited = callback(renderer);
        return is_finish || is_exited;
    }

private:
    // 状态变量
    double distance = 0;

    // 配置变量
    QColor color;
    QPainterPath path;
    QPointF origin;
    double speed;
    double max_distance;

    std::function<bool(const Renderer&)> callback;
};

}
