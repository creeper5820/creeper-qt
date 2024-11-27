#pragma once

#include "creeper-qt/setting/theme.hh"
#include "creeper-qt/utility/pid.hh"
#include "creeper-qt/widget/widget.hh"

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
        : Extension(parent) {
        connect(&animationTimer_, &QTimer::timeout, [this] { update(); });
        reloadTheme();
    }

    void reloadTheme() override {
        buttonColor_ = Theme::color("primary100");
        waterColor_ = Theme::color("primary300");
        textColor_ = Theme::color("text");
    }

    void setWaterColor(uint32_t color) {
        waterColor_ = color;
        reloadTheme();
    }
    void setWaterColor(QColor color) {
        waterColor_ = color.value();
        reloadTheme();
    }
    void setRadiusRatio(float ratio) { radiusRatio_ = ratio; }

    // Water ripple animation
    void enableAnimation() { waterRippleAnimation_ = true; }
    void disableAnimation() { waterRippleAnimation_ = false; }
    void setDiffusionStep(int step) { diffusionStep = step; }

protected:
    void paintEvent(QPaintEvent* event) override {
        auto painter = QPainter(this);
        buttonPaintEvent(painter);
        waterRippleAnimationPaintEvent(painter);
        textPaintEvent(painter);
        checkAnimation();
    }

    void mouseReleaseEvent(QMouseEvent* event) override {
        if (event->button() & Qt::LeftButton) {
            if (waterRippleAnimation_) {
                animationEvents_.emplace_back(event->pos(), 0);
                if (!animationTimer_.isActive()) animationTimer_.start(refreshIntervalMs_);
            }
        }
        Extension::mouseReleaseEvent(event);
    }

    void enterEvent(QEnterEvent* event) override {
        mouseHover_ = true;
        if (!animationTimer_.isActive()) animationTimer_.start(refreshIntervalMs_);
    }

    void leaveEvent(QEvent* event) override {
        mouseHover_ = false;
        if (!animationTimer_.isActive()) animationTimer_.start(refreshIntervalMs_);
    }

    void buttonPaintEvent(QPainter& painter) {
        const auto width = Extension::width();
        const auto height = Extension::height();

        auto roundRectPath = QPainterPath();
        roundRectPath.addRoundedRect(
            0, 0, width, height, radiusRatio_ * height, radiusRatio_ * height);

        auto target = mouseHover_ ? mouseHoverOpacity : mouseLeaveOpacity;
        opacity_ = updateWithPid(opacity_, target, 0.1);

        painter.setPen(Qt::NoPen);
        painter.setBrush({ buttonColor_ });
        painter.setOpacity(opacity_);
        painter.setRenderHint(QPainter::Antialiasing, true);

        painter.drawPath(roundRectPath);
    }

    void textPaintEvent(QPainter& painter) {
        painter.setPen(QColor(textColor_));
        painter.setFont(QFont("monospace", 12, QFont::Bold));
        painter.setOpacity(0.7);
        painter.drawText(0, 0, width(), height(), Qt::AlignCenter, text());
    }

    void waterRippleAnimationPaintEvent(QPainter& painter) {
        if (!waterRippleAnimation_) return;

        const auto width = Extension::width();
        const auto height = Extension::height();

        const auto maxDistance = 2 * std::max(width, height);

        auto roundRectPath = QPainterPath();
        roundRectPath.addRoundedRect(
            0, 0, width, height, radiusRatio_ * height, radiusRatio_ * height);

        painter.setPen(Qt::NoPen);
        painter.setBrush({ waterColor_ });
        painter.setRenderHint(QPainter::Antialiasing, true);

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

    void checkAnimation() {
        if (std::abs(opacity_ - (mouseHover_ ? mouseHoverOpacity : mouseLeaveOpacity)) < 0.001
            && animationEvents_.empty())
            animationTimer_.stop();
    }

private:
    // For Animation
    bool waterRippleAnimation_ = true;
    bool mouseHover_ = false;

    int diffusionStep = 5;
    double opacity_ = mouseLeaveOpacity;

    double radiusRatio_ = 0.15;

    uint32_t waterColor_;
    uint32_t buttonColor_;
    uint32_t textColor_;

    QTimer animationTimer_;

    std::vector<std::tuple<QPointF, int>> animationEvents_;

    constexpr static inline double mouseHoverOpacity = 1.0;
    constexpr static inline double mouseLeaveOpacity = 0.6;
};

}