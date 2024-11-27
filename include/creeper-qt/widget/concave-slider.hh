#pragma once

#include "creeper-qt/utility/pid.hh"
#include "creeper-qt/widget/widget.hh"

#include <qpainter.h>
#include <qpicture.h>
#include <qslider.h>

namespace creeper {

class ConcaveSlider : public Extension<QAbstractSlider> {
    Q_OBJECT
public:
    explicit ConcaveSlider(QWidget* parent = nullptr)
        : Extension(parent) {
        setTracking(true);
        reloadTheme();
    }

    void reloadTheme() override {
        foreground_ = Theme::color("primary400");
        lineColor_ = Theme::color("primary050");
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        auto painter = QPainter(this);
        auto sliderCanvas = QPicture();
        makeSliderCanvas(sliderCanvas, width(), height());
        painter.drawPicture(0, 0, sliderCanvas);
    }

private:
    void makeSliderCanvas(QPicture& picture, int width, int height) {
        auto painter = QPainter(&picture);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(Qt::NoPen);

        const auto radius = 0.5 * height;
        const auto lineRadius = 0.2 * radius;
        const auto x0 = radius - lineRadius, x1 = width - radius + lineRadius;
        const auto y0 = radius - lineRadius, y1 = height - radius + lineRadius;
        painter.setBrush({ lineColor_ });
        painter.drawRoundedRect({ QPointF(x0, y0), QPointF(x1, y1) }, lineRadius, lineRadius);

        const auto value = Extension::value();
        const auto min = Extension::minimum();
        const auto max = Extension::maximum();
        const auto ratio = static_cast<double>(value - min) / (max - min);
        const auto length = 2 * radius + ratio * (width - 2 * radius);
        painter.setBrush({ foreground_ });
        painter.drawRoundedRect(QRectF(0, 0, length, height), radius, radius);
    }

private:
    uint32_t foreground_;
    uint32_t lineColor_;
};

}