#pragma once

#include "creeper-qt/utility/pid.hh"
#include "creeper-qt/widget/widget.hh"

#include <qevent.h>
#include <qpainter.h>
#include <qpicture.h>
#include <qtimer.h>

namespace creeper {

class ConcaveSlider : public Extension<QWidget> {
    Q_OBJECT
public:
    explicit ConcaveSlider(QWidget* parent = nullptr)
        : Extension(parent) {
        connect(&timer_, &QTimer::timeout, [this] { update(); });
        reloadTheme();
    }

    void reloadTheme() override {
        foreground_ = Theme::color("primary400");
        lineColor_ = Theme::color("primary050");
    }

    ConcaveSlider& setRange(int minimum, int maximum) {
        minimum_ = minimum, maximum_ = maximum;
        return *this;
    }
    int minimum() const { return minimum_; }
    int maximum() const { return maximum_; }

    ConcaveSlider& setValue(int value) {
        if (!timer_.isActive()) timer_.start(refreshIntervalMs_);
        value_ = value;
        return *this;
    }
    int value() const { return value_; }

protected:
    void paintEvent(QPaintEvent* event) override {
        auto painter = QPainter(this);
        auto sliderCanvas = QPicture();
        makeSliderCanvas(sliderCanvas, width(), height());
        painter.drawPicture(0, 0, sliderCanvas);
    }
    void mouseMoveEvent(QMouseEvent* event) override { syncValueFromMouseEvent(*event); }
    void mousePressEvent(QMouseEvent* event) override { syncValueFromMouseEvent(*event); }

private:
    double radius() const { return width() > height() ? height() / 2. : width() / 2.; }

    void syncValueFromMouseEvent(QMouseEvent& event) {
        const auto ratio = (event.pos().x() - radius()) / (width() - radius() * 2.);
        setValue(std::clamp(ratio, 0.0, 1.0) * (maximum_ - minimum_) + minimum_);
    }

    void makeSliderCanvas(QPicture& picture, int width, int height) {
        auto painter = QPainter(&picture);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);

        const auto radius = 0.5 * height, lineRadius = 0.2 * radius;
        const auto x0 = radius - lineRadius, x1 = width - radius + lineRadius;
        const auto y0 = radius - lineRadius, y1 = height - radius + lineRadius;
        painter.setBrush({ lineColor_ });
        painter.drawRoundedRect({ QPointF(x0, y0), QPointF(x1, y1) }, lineRadius, lineRadius);

        const auto ratio = static_cast<double>(value_ - minimum_) / (maximum_ - minimum_);
        const auto target = 2 * radius + ratio * (width - 2 * radius);
        static auto length = target;
        length = updateWithPid(length, target, 0.1);
        painter.setBrush({ foreground_ });
        painter.drawRoundedRect(QRectF(0, 0, length, height), radius, radius);

        if (std::abs(length - target) < 0.001) timer_.stop();
    }

private:
    QTimer timer_;

    uint32_t foreground_;
    uint32_t lineColor_;

    int value_ = 0;
    int minimum_ = 0;
    int maximum_ = 100;
};

}