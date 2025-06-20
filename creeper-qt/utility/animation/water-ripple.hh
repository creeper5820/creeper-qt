#pragma once

#include "creeper-qt/utility/animation/core.hh"
#include "creeper-qt/utility/painter/helper.hh"

#include <qdebug.h>
#include <qevent.h>
#include <qpaintdevice.h>
#include <qpainter.h>
#include <qpainterpath.h>

namespace creeper::util::animation {

struct WaterRipple final : IAnimation {
public:
    struct Result {
        QPixmap pixmap;
        uint unique_id;
        bool is_finish;
    };

    WaterRipple(const QColor& color, const QPainterPath& path, const QPointF& origin, double speed,
        double max_distance, const std::function<void(std::unique_ptr<Result>)>& callback)
        : color(color)
        , path(path)
        , origin(origin)
        , speed(speed)
        , max_distance(max_distance)
        , callback(callback) {
        unique_id = static_unique_id++;
    }

    bool update() override {
        auto result = std::make_unique<Result>();

        result->pixmap = QPixmap { path.boundingRect().size().toSize() };
        result->pixmap.fill(Qt::transparent);

        const auto is_finish = (distance += speed) > max_distance;
        if (!is_finish) {
            auto painter = QPainter { &result->pixmap };
            PainterHelper { painter }
                .set_clip_path(path)
                .set_opacity(1. - distance / max_distance)
                .ellipse(color, Qt::transparent, 0, origin, distance, distance)
                .done();
        }

        result->is_finish = is_finish;
        result->unique_id = unique_id;

        callback(std::move(result));

        return is_finish;
    }

private:
    static inline uint static_unique_id = 0;
    uint unique_id;

    // 状态变量
    double distance = 0;

    // 配置变量
    QColor color;
    QPainterPath path;
    QPointF origin;
    double speed;
    double max_distance;

    std::function<void(std::unique_ptr<Result>)> callback;
};

class WaterRippleContainer {
public:
    void append(std::unique_ptr<WaterRipple::Result> result) {
        water_ripples[result->unique_id] = std::move(result);
    }

    void render(QPainter& painter, const QRectF& src, const QRectF& dst) {
        std::erase_if(water_ripples, [](const auto& p) { return p.second->is_finish; });
        std::ranges::for_each(water_ripples, [&painter, &src, &dst](const auto& p) {
            painter.drawPixmap(dst, p.second->pixmap, src);
        });
    }

private:
    std::unordered_map<uint, std::unique_ptr<WaterRipple::Result>> water_ripples;
};

}
