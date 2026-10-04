#pragma once

#include <qpainter.h>
#include <qpainterpath.h>

#include "creeper-qt/utility/animation/animatable.hh"
#include "creeper-qt/utility/animation/state/pid.hh"
#include "creeper-qt/utility/animation/transition.hh"
#include "creeper-qt/utility/wrapper/common.hh"
#include "creeper-qt/utility/wrapper/dsl.hh"
#include "creeper-qt/utility/wrapper/widget.hh"

namespace creeper {

class MixerMask : public QWidget, public DSL {
public:
    explicit MixerMask(auto&&... props)
        : MixerMask { } {
        construct_with(std::forward<decltype(props)>(props)...);
    }

    MixerMask() noexcept
        : QWidget { }
        , animatable { *this } {

        QWidget::setAttribute(Qt::WA_TransparentForMouseEvents);

        mask_frame.fill(Qt::transparent);
        {
            auto state = std::make_shared<PidState<double>>();

            state->config.kp      = 05.0;
            state->config.ki      = 00.0;
            state->config.kd      = 00.0;
            state->config.epsilon = 1e-3;

            mask_radius = make_transition(animatable, std::move(state));
        }
    }

    auto initiateAnimation(QPoint const& point) noexcept {
        mask_frame.fill(Qt::transparent);

        auto* widget = parentWidget();
        if (widget == nullptr) return;

        mask_radius->snap_to(0.);
        mask_radius->transition_to(1.);

        mask_point = point;
        mask_frame = widget->grab();

        update_animation = true;
        QWidget::setFixedSize(widget->size());
    }
    auto initiateAnimation(int x, int y) noexcept {
        // Forward Point
        initiateAnimation(QPoint { x, y });
    }

protected:
    auto paintEvent(QPaintEvent* e) -> void override {
        if (!update_animation) return;

        auto const w = QWidget::width();
        auto const h = QWidget::height();
        auto const x = std::sqrt(w * w + h * h);

        auto painter = QPainter { this };

        auto const radius = double { *mask_radius * x };
        auto const round  = [&] {
            auto path = QPainterPath { };
            path.addRect(QWidget::rect());

            auto inner = QPainterPath();
            inner.addEllipse(mask_point, radius, radius);

            return path.subtracted(inner);
        }();
        painter.setClipPath(round);
        painter.setClipping(true);

        painter.drawPixmap(QWidget::rect(), mask_frame);

        if (std::abs(*mask_radius - 1.) < 1e-2) {
            update_animation = false;
        }
    }

private:
    QPixmap mask_frame;
    QPointF mask_point;

    bool update_animation = false;

    Animatable animatable;
    std::unique_ptr<TransitionValue<PidState<double>>> mask_radius;
};

}

namespace creeper::mixer::pro {

using namespace common::pro;
using namespace widget::pro;

struct SetMixerMask {
    MixerMask*& mask;

    explicit SetMixerMask(MixerMask*& mask) noexcept
        : mask { mask } { }

    friend auto dsl_invoke(auto& self, const SetMixerMask& prop) -> void {
        prop.mask = new MixerMask { Parent { &self } };
    }
};

}
