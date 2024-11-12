#pragma once

#include "setting/theme.hh"
#include "widget/widget.hh"

#include <qevent.h>
#include <qpainter.h>
#include <qpainterpath.h>
#include <qpropertyanimation.h>
#include <qpushbutton.h>
#include <qtimer.h>

namespace creeper {

class PushButton : public Extension<QPushButton> {
    Q_OBJECT
public:
    PushButton(QWidget* parent = nullptr)
        : Extension("default", parent) {
        animationTimer_ = std::make_unique<QTimer>();
        connect(animationTimer_.get(), &QTimer::timeout, [this] {
            Extension::update();
        });

        reloadTheme();
    }

    void reloadTheme() override {
        Extension::loadStyleFromFile(Theme::qss("push-button"));
        waterColor_ = Theme::color("primary300");
    }

    // Water ripple animation

    void enableAnimation() {
        waterRippleAnimation_ = true;
    }
    void disableAnimation() {
        waterRippleAnimation_ = false;
    }
    void setRefreshTime(std::chrono::milliseconds ms) {
        refreshTime_ = ms;
    }
    void setDiffusionStep(int step) {
        diffusionStep = step;
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        Extension::paintEvent(event);
        waterRippleAnimationPaintEvent(event);
    }

    void waterRippleAnimationPaintEvent(QPaintEvent* event) {
        if (!waterRippleAnimation_)
            return;

        const auto width = Extension::width();
        const auto height = Extension::height();

        const auto maxDistance = 2 * std::max(width, height);

        auto roundRectPath = QPainterPath();
        roundRectPath.addRoundedRect(0, 0, width, height, 0.1 * width, 0.1 * height);

        auto painter = QPainter(this);
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(waterColor_));
        painter.setRenderHint(QPainter::Antialiasing, true);

        if (animationEvents_.empty())
            animationTimer_->stop();

        for (int index = 0; auto& [point, distance] : animationEvents_) {
            painter.setOpacity(0.3 * (1 - static_cast<double>(distance) / maxDistance));

            auto ellipsePath = QPainterPath();
            ellipsePath.addEllipse(point, distance, distance);

            painter.drawPath(ellipsePath.intersected(roundRectPath));

            if (distance > maxDistance) {
                animationEvents_.erase(animationEvents_.begin() + index);
                index--;
            }
            distance += diffusionStep, index++;
        }
    }

    void mouseReleaseEvent(QMouseEvent* event) override {
        if (event->button() & Qt::LeftButton) {
            if (waterRippleAnimation_) {
                animationEvents_.emplace_back(event->pos(), 0);
                if (!animationTimer_->isActive())
                    animationTimer_->start(refreshTime_);
            }
        }
        Extension::mouseReleaseEvent(event);
    }

private:
    // For Animation
    bool waterRippleAnimation_ = true;

    int diffusionStep = 5;

    uint32_t waterColor_ = 0x000000;

    std::chrono::milliseconds refreshTime_ { 10 };

    std::unique_ptr<QTimer> animationTimer_;

    std::vector<std::tuple<QPoint, int>> animationEvents_;
};

}