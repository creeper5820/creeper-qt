#pragma once

#include "../widget/wave-circle.hh"
#include "../widget/widget.hh"

#include <qpainter.h>

namespace creeper {

class ScallopClock : public Extension<QWidget> {
    Q_OBJECT
public:
    explicit ScallopClock(QWidget* parent = nullptr)
        : Extension(parent) {
        waveCircle_.setFlange(12);
        waveCircle_.setFlangeRatio(0.9);
        waveCircle_.setFlangeRadius(20);
        waveCircle_.setLineWidth(2);
        reloadTheme();
    }

    void setRadius(int radius) {
        radius_ = radius;
        waveCircle_.setRadius(radius_);
        waveCircle_.setFlangeRadius(20. * radius / 150);
        waveCircle_.setLineWidth(2. * radius / 150);
        setFixedSize(radius_ * 2, radius_ * 2);
    }

    void setAngle(double secondAngle, double minuteAngle, double hourAngle) {
        secondAngle_ = secondAngle;
        minuteAngle_ = minuteAngle;
        hourAngle_ = hourAngle;
        update();
    }

    void reloadTheme() override { waveCircle_.setLineColor(Theme::color("primary200")); }

protected:
    void paintEvent(QPaintEvent* event) override {
        const auto center = QPoint(width() / 2, height() / 2);

        auto painter = QPainter { this };
        painter.setOpacity(1);
        painter.setRenderHint(QPainter::Antialiasing);

        const auto theSecondAngle = 270 + secondAngle_;
        const auto secondLength = 0.6 * radius_;
        const double secondX = secondLength * std::cos(theSecondAngle * std::numbers::pi / 180);
        const double secondY = secondLength * std::sin(theSecondAngle * std::numbers::pi / 180);
        const auto secondEnd = center + QPoint(secondX, secondY);
        const auto secondColor = QColor { Theme::color("primary300") };
        painter.setPen(Qt::NoPen);
        painter.setBrush({ secondColor });
        painter.drawEllipse(secondEnd, 10, 10);

        const auto theMinuteAngle = 270 + minuteAngle_;
        const auto minuteLength = 0.4 * radius_;
        const double minuteX = minuteLength * std::cos(theMinuteAngle * std::numbers::pi / 180);
        const double minuteY = minuteLength * std::sin(theMinuteAngle * std::numbers::pi / 180);
        const auto minuteEnd = center + QPoint(minuteX, minuteY);
        const auto minuteColor = QColor { Theme::color("primary300") };
        const auto minutePen = QPen { minuteColor, 20, Qt::SolidLine, Qt::RoundCap };
        painter.setPen(minutePen);
        painter.setBrush(Qt::NoBrush);
        painter.drawLine(center, minuteEnd);

        const auto theHourAngle = 270 + hourAngle_;
        const auto hourLength = 0.3 * radius_;
        const double hourX = hourLength * std::cos(theHourAngle * std::numbers::pi / 180);
        const double hourY = hourLength * std::sin(theHourAngle * std::numbers::pi / 180);
        const auto hourEnd = center + QPoint(hourX, hourY);
        const auto hourColor = QColor { Theme::color("primary400") };
        const auto hourPen = QPen { hourColor, 20, Qt::SolidLine, Qt::RoundCap };
        painter.setPen(hourPen);
        painter.setBrush(Qt::NoBrush);
        painter.drawLine(center, hourEnd);
    };

private:
    WaveCircle waveCircle_ { this };

    double secondAngle_ = 100;
    double minuteAngle_ = 100;
    double hourAngle_ = 100;

    double radius_ = 100;
};

}
