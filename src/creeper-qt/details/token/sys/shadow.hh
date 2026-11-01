#pragma once

#include "creeper-qt/details/token/sys/unit.hh"

#include <qgraphicseffect.h>

namespace creeper {

/// @brief 按 elevation（dp）投射阴影
///
/// Compose 在 Android 上使用系统的环境光 + 主光源双阴影，Qt 只有单层投影，
/// 这里用单层投影近似：模糊半径 = 2 × elevation，纵向偏移 = elevation / 2。
struct Elevation : QGraphicsDropShadowEffect {
    static constexpr auto kShadowOpacity = 0.3;

    explicit Elevation(Dp level = { }, QObject* parent = nullptr)
        : QGraphicsDropShadowEffect { parent } {
        setLevel(level);
        setShadowColor(Qt::black);
    }

    auto setLevel(Dp value) -> void {
        level = value;
        QGraphicsDropShadowEffect::setBlurRadius(level.value * 2.0);
        QGraphicsDropShadowEffect::setOffset(0, level.value / 2.0);
        QGraphicsEffect::setEnabled(level.value > 0);
    }

    auto getLevel() const -> Dp { return level; }

    auto setShadowColor(const QColor& color) -> void {
        auto shadow = color;
        shadow.setAlphaF(kShadowOpacity);
        QGraphicsDropShadowEffect::setColor(shadow);
    }

private:
    Dp level { };
};

}
