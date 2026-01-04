#pragma once
#include "creeper-qt/utility/animation/animatable.hh"
#include "creeper-qt/utility/animation/state/pid.hh"
#include "creeper-qt/utility/animation/transition.hh"
#include "creeper-qt/utility/theme/theme.hh"
#include "creeper-qt/utility/wrapper/widget.hh"

#include <qpainter.h>
#include <qpainterpath.h>
#include <qpointer.h>

namespace creeper::mixer::internal {

class MixerMask : public QWidget {
public:
    explicit MixerMask(auto* widget) noexcept
        : QWidget { widget } {

        QWidget::setAttribute(Qt::WA_TransparentForMouseEvents);

        mask_frame.fill(Qt::transparent);
        {
            auto& state = mask_radius.get_state();

            state.config.kp      = 05.0;
            state.config.ki      = 00.0;
            state.config.kd      = 00.0;
            state.config.epsilon = 1e-3;
        }
    }

    auto initiate_animation(QPoint const& point) noexcept {
        mask_frame.fill(Qt::transparent);

        auto* widget = parentWidget();
        if (widget == nullptr) return;

        mask_radius.snap_to(0.);
        mask_radius.transition_to(1.);

        mask_point = point;
        mask_frame = widget->grab();

        update_animation = true;
        QWidget::setFixedSize(widget->size());
    }
    auto initiate_animation(int x, int y) noexcept {
        // Forward Point
        initiate_animation(QPoint { x, y });
    }

protected:
    auto paintEvent(QPaintEvent* e) -> void override {
        if (!update_animation) return;

        auto const w = QWidget::width();
        auto const h = QWidget::height();
        auto const x = std::sqrt(w * w + h * h);

        auto painter = QPainter { this };

        auto const radius = double { mask_radius * x };
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

        if (std::abs(mask_radius - 1.) < 1e-2) {
            update_animation = false;
        }
    }

private:
    QPixmap mask_frame;
    QPointF mask_point;

    bool update_animation = false;

    Animatable animatable { *this };
    TransitionValue<PidState<double>> mask_radius { animatable };
};

}
namespace creeper {

using MixerMask = mixer::internal::MixerMask;

}
namespace creeper::widget::pro {

struct BindAndNewMixerMask : Token {
    mixer::internal::MixerMask*& mask;

    // @param mask 空指针，未构造，作为返回值
    explicit BindAndNewMixerMask(auto*& mask)
        : mask { mask } { }
    auto apply(auto& self) noexcept {
        //
        mask = new MixerMask { &self };
    }
};

struct CreateMixerMask : Token {
    ThemeManager& manager;

    explicit CreateMixerMask(ThemeManager& manager)
        : manager { manager } { }

    auto apply(auto& self) noexcept {
        // 所有权移交 Parent
        auto mask = QPointer<MixerMask> { new MixerMask { &self } };
        manager.append_begin_callback([=](const ThemeManager& manager) {
            if (!mask.isNull() && manager.applied()) {
                auto point = mask->mapFromGlobal(QCursor::pos());
                mask->initiate_animation(point);
            }
        });
    }
};

}
