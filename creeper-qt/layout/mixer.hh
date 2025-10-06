#pragma once

#include <qpainter.h>
#include <qpainterpath.h>

#include "creeper-qt/utility/animation/animatable.hh"
#include "creeper-qt/utility/animation/state/pid.hh"
#include "creeper-qt/utility/animation/transition.hh"
#include "creeper-qt/utility/wrapper/widget.hh"

namespace creeper::mixer::internal {

class MixerMask : public QWidget {
public:
    explicit MixerMask(auto* widget) noexcept
        : QWidget { widget }
        , animatable { *this } {

        mask_frame.fill(Qt::transparent);
        {
            auto state = std::make_shared<PidState<double>>();

            state->config.kp      = 05.0;
            state->config.ki      = 00.0;
            state->config.kd      = 00.0;
            state->config.epsilon = 1e-3;

            mask_radius = make_transition(animatable, std::move(state));
        }
        QWidget::setVisible(false);
        QWidget::setAttribute(Qt::WA_TransparentForMouseEvents);
    }

    auto initiate_animation(QPoint const& point) noexcept {
        mask_frame.fill(Qt::transparent);

        auto* widget = parentWidget();
        if (widget == nullptr) return;

        mask_radius->snap_to(0.);
        mask_radius->transition_to(1.);

        mask_point = point;
        mask_frame = widget->grab();

        QWidget::setFixedSize(widget->size());
        QWidget::setVisible(true);
    }
    auto initiate_animation(int x, int y) noexcept {
        // Forward Point
        initiate_animation(QPoint { x, y });
    }

protected:
    auto paintEvent(QPaintEvent* e) -> void override {
        if (std::abs(*mask_radius - 1.) > 1e-2) {
            auto const w = QWidget::width();
            auto const h = QWidget::height();
            auto const x = std::sqrt(w * w + h * h);

            auto painter = QPainter { this };

            auto const radius = double { *mask_radius * x };
            auto const round  = [&] {
                auto path = QPainterPath {};
                path.addRect(QWidget::rect());

                auto inner = QPainterPath();
                inner.addEllipse(mask_point, radius, radius);

                return path.subtracted(inner);
            }();
            painter.setClipPath(round);
            painter.setClipping(true);

            painter.drawPixmap(QWidget::rect(), mask_frame);
        } else {
            QWidget::setVisible(false);
        }
    }

private:
    QPixmap mask_frame;
    QPointF mask_point;

    Animatable animatable;
    std::unique_ptr<TransitionValue<PidState<double>>> mask_radius;
};

}
namespace creeper::mixer::pro {

struct SetMixerMask : widget::pro::Token {
    internal::MixerMask*& mask;
    explicit SetMixerMask(auto*& mask)
        : mask { mask } { }
    auto apply(auto& self) noexcept {
        //
        mask = new internal::MixerMask { &self };
    }
};

}
namespace creeper {

using MixerMask = mixer::internal::MixerMask;

}
