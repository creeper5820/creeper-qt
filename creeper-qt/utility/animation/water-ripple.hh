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
    explicit WaterRipple(QPaintDevice& self, const QPainterPath& path, const QPointF& origin,
        const QColor& color, double max_distance, double speed, double opacity)
        : self(self)
        , path(path)
        , origin(origin)
        , max_distance(max_distance)
        , speed(speed)
        , opacity(opacity)
        , color(color) { }

    bool update(const QPaintEvent&) override {
        auto painter = QPainter { &self };
        PainterHelper { painter }
            .set_clip_path(path)
            .set_opacity(opacity * (1. - distance / max_distance))
            .ellipse(color, Qt::transparent, 0, origin, distance, distance);

        return (distance += speed) > max_distance;
    }

private:
    // 状态变量
    double distance = 0;

    // 配置变量
    QPaintDevice& self;
    QPainterPath path;
    QPointF origin;
    double max_distance;
    double speed;
    double opacity;
    QColor color;
};

}
