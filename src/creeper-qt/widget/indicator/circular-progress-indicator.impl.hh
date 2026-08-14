#pragma once
#include "circular-progress-indicator.hh"

#include "creeper-qt/utility/animation/animatable.hh"
#include "creeper-qt/utility/animation/state/cyclic.hh"
#include "creeper-qt/utility/animation/state/pid.hh"
#include "creeper-qt/utility/animation/transition.hh"

#include <cmath>
#include <qpainter.h>

using namespace creeper::circular_progress_indicator::details;

struct CircularProgressIndicator::Impl {
    bool indeterminate = false;

    double progress     = 0.0;
    double stroke_width = 0.0;

    QColor indicator_color = Qt::black;
    QColor track_color     = Qt::gray;

    Animatable animatable;

    std::unique_ptr<TransitionValue<PidState<double>>> position;
    std::unique_ptr<TransitionValue<CyclicState<double>>> spin;

    explicit Impl(CircularProgressIndicator& self) noexcept
        : animatable(self) {
        {
            auto state = std::make_shared<PidState<double>>();

            state->config.kp      = 15.0;
            state->config.epsilon = 1e-4;

            position = make_transition(animatable, std::move(state));
        }
        {
            auto state = std::make_shared<CyclicState<double>>();

            state->config.speed = 1.0 / 1.3;

            spin = make_transition(animatable, std::move(state));
        }
    }

    void set_color_scheme(CircularProgressIndicator& self, const ColorScheme& scheme) {
        indicator_color = scheme.primary;
        track_color     = scheme.secondary_container;
        self.update();
    }

    void set_progress(CircularProgressIndicator& self, double value) noexcept {
        progress = std::clamp(value, 0.0, 1.0);
        position->transition_to(progress);
    }

    void set_indeterminate(CircularProgressIndicator& self, bool on) noexcept {
        if (indeterminate == on) return;

        indeterminate = on;
        if (on) {
            spin->transition_to(spin->get_value());
        } else {
            spin->snap_to(0.0);
        }
        self.update();
    }

    void paint_event(CircularProgressIndicator& self, const QPaintEvent&) {

        const auto side  = std::min(self.width(), self.height());
        const auto width = stroke_width > 0.0 ? stroke_width : side * 0.1;

        const auto half   = width / 2.0;
        const auto square = QRectF {
            (self.width() - side) / 2.0 + half,
            (self.height() - side) / 2.0 + half,
            side - width,
            side - width,
        };

        // Qt 角度制：1/16 度，0° 指向三点钟方向，逆时针为正
        const auto unit  = 16.0;
        const auto begin = 90.0 * unit;

        auto start_angle = begin;
        auto span_angle  = -360.0 * unit * *position;

        if (indeterminate) {
            constexpr auto pi        = std::numbers::pi;
            constexpr auto sweep_min = 10.0;
            constexpr auto sweep_max = 270.0;

            const auto phase     = *spin;
            const auto rotation  = phase * 360.0;
            const auto oscillate = 0.5 * (1.0 - std::cos(2.0 * pi * phase));
            const auto sweep     = sweep_min + (sweep_max - sweep_min) * oscillate;

            start_angle = begin - rotation * unit;
            span_angle  = -sweep * unit;
        }

        auto painter = QPainter { &self };
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setBrush(Qt::NoBrush);

        painter.setPen(QPen { track_color, width, Qt::SolidLine, Qt::RoundCap });
        painter.drawEllipse(square);

        painter.setPen(QPen { indicator_color, width, Qt::SolidLine, Qt::RoundCap });
        painter.drawArc(square, static_cast<int>(start_angle), static_cast<int>(span_angle));
    }
};
