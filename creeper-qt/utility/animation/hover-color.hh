#pragma once

#include "utility/animation/core.hh"
#include "utility/animation/math.hh"
#include "utility/painter/helper.hh"
#include <qevent.h>
#include <qpaintdevice.h>
#include <qpainter.h>
#include <qpainterpath.h>

namespace creeper::util::animation {

struct RGBA {
    double r = 0.;
    double g = 0.;
    double b = 0.;
    double a = 0.;
};

struct GradientColor final : IAnimation {
public:
    double update_ratio;
    RGBA begin;
    RGBA final;

    std::function<void()> finish_callback;

    bool update(const QPaintEvent&) {
        const auto [r, g, b, a] = begin;
        return true;
    }
};

}
