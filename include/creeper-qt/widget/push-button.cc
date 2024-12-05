#include "push-button.hh"

#include "creeper-qt/utility/pid.hh"

#include <qevent.h>
#include <qpainter.h>
#include <qpainterpath.h>
#include <qtimer.h>

using namespace creeper;

struct PushButton::Impl {
public:
    void buttonPaintEvent(QPainter& painter, int width, int height) {
        auto roundRectPath = QPainterPath();
        roundRectPath.addRoundedRect(
            0, 0, width, height, radiusRatio_ * height, radiusRatio_ * height);

        painter.setPen(Qt::NoPen);
        painter.setBrush({ drawBackground_ ? buttonColor_ : 0xffffffff });
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setOpacity(1);

        painter.drawPath(roundRectPath);
    }

    void buttonGradientPaintEvent(QPainter& painter, int width, int height) {
        auto target = mouseHover_ ? mouseHoverOpacity : mouseLeaveOpacity;
        opacity_ = updateWithPid(opacity_, target, 0.1);

        painter.setPen(Qt::NoPen);
        painter.setBrush({ waterColor_ });
        painter.setOpacity(opacity_);
        painter.setRenderHint(QPainter::Antialiasing, true);

        painter.drawRoundedRect(0, 0, width, height, radiusRatio_ * height, radiusRatio_ * height);
    }

    /// @todo 适配多行文字或者不能显示完全的情况
    void textPaintEvent(
        QPainter& painter, const QFont& font, int width, int height, const QString& text) {
        painter.setPen(textColor_);
        painter.setFont(font);
        painter.setOpacity(0.7);
        painter.drawText(0, 0, width, height, Qt::AlignCenter, text);
    }

    void waterRippleAnimationPaintEvent(QPainter& painter, int width, int height) {
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

public:
    // For Animation
    bool waterRippleAnimation_ = true;
    bool mouseHover_ = false;
    bool drawBackground_ = true;

    int diffusionStep = 5;
    double opacity_ = mouseLeaveOpacity;

    double radiusRatio_ = 0.15;

    uint32_t waterColor_;
    uint32_t buttonColor_;
    uint32_t textColor_;

    QTimer animationTimer_;

    std::vector<std::tuple<QPointF, int>> animationEvents_;

    constexpr static inline double mouseHoverOpacity = 0.5;
    constexpr static inline double mouseLeaveOpacity = 0.0;
};

PushButton::PushButton(QWidget* parent)
    : Extension(parent)
    , pimpl_(new Impl) {
    connect(&pimpl_->animationTimer_, &QTimer::timeout, [this] { update(); });
    reloadTheme();
}

PushButton::~PushButton() { delete pimpl_; }

void PushButton::reloadTheme() {
    pimpl_->buttonColor_ = Theme::color("primary100");
    pimpl_->waterColor_ = Theme::color("primary300");
    pimpl_->textColor_ = Theme::color("text");
}
void PushButton::setWaterColor(uint32_t color) {
    pimpl_->waterColor_ = color;
    reloadTheme();
}
void PushButton::setRadiusRatio(float ratio) { pimpl_->radiusRatio_ = ratio; }

void PushButton::disableBackground() { pimpl_->drawBackground_ = false; }
void PushButton::enableBackground() { pimpl_->drawBackground_ = true; }

// Water ripple animation
void PushButton::enableAnimation() { pimpl_->waterRippleAnimation_ = true; }
void PushButton::disableAnimation() { pimpl_->waterRippleAnimation_ = false; }
void PushButton::setDiffusionStep(int step) { pimpl_->diffusionStep = step; }

/// PROTECTED
void PushButton::paintEvent(QPaintEvent* event) {
    auto painter = QPainter(this);
    pimpl_->buttonPaintEvent(painter, width(), height());
    pimpl_->buttonGradientPaintEvent(painter, width(), height());
    if (pimpl_->waterRippleAnimation_) {
        pimpl_->waterRippleAnimationPaintEvent(painter, width(), height());
        pimpl_->checkAnimation();
    }
    pimpl_->textPaintEvent(painter, font(), width(), height(), text());
}

void PushButton::mouseReleaseEvent(QMouseEvent* event) {
    if (event->button() & Qt::LeftButton) {
        if (pimpl_->waterRippleAnimation_) {
            pimpl_->animationEvents_.emplace_back(event->pos(), 0);
            if (!pimpl_->animationTimer_.isActive())
                pimpl_->animationTimer_.start(refreshIntervalMs_);
        }
    }
    Extension::mouseReleaseEvent(event);
}

void PushButton::enterEvent(QEvent* event) {
    pimpl_->mouseHover_ = true;
    if (!pimpl_->animationTimer_.isActive()) //
        pimpl_->animationTimer_.start(refreshIntervalMs_);
    Extension::enterEvent(event);
}

void PushButton::leaveEvent(QEvent* event) {
    pimpl_->mouseHover_ = false;
    if (!pimpl_->animationTimer_.isActive()) //
        pimpl_->animationTimer_.start(refreshIntervalMs_);
    Extension::leaveEvent(event);
}