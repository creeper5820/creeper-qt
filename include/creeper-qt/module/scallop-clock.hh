#pragma once

#include "../widget/wave-circle.hh"
#include "../widget/widget.hh"

#include <qpainter.h>

namespace creeper {

namespace internal {
    class ClockPointer : public Extension<QWidget> {
        Q_OBJECT
    public:
        explicit ClockPointer(QWidget* parent = nullptr)
            : Extension(parent) {
            reloadTheme();
        }
        void reloadTheme() override {
            secondColor_ = Theme::color("primary300");
            minuteColor_ = Theme::color("primary300");
            hourColor_ = Theme::color("primary400");
        }
        void setAngle(double secondAngle, double minuteAngle, double hourAngle) {
            secondAngle_ = secondAngle;
            minuteAngle_ = minuteAngle;
            hourAngle_ = hourAngle;
            update();
        }
        void setRadius(double radius) {
            radius_ = radius;
            setFixedSize(2 * radius, 2 * radius);
        }

    protected:
        void paintEvent(QPaintEvent* event) override {
            using std::numbers::pi;

            const auto center = QPoint(width() / 2, height() / 2);

            auto painter = QPainter { this };
            painter.setOpacity(1);
            painter.setRenderHint(QPainter::Antialiasing);

            const auto secondAngle = 270 + secondAngle_;
            const auto secondLength = 0.6 * radius_;
            const double secondX = secondLength * std::cos(secondAngle * pi / 180);
            const double secondY = secondLength * std::sin(secondAngle * pi / 180);
            const auto secondEnd = center + QPoint(secondX, secondY);
            painter.setBrush({ secondColor_ });
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(secondEnd, 10, 10);

            const auto minuteAngle = 270 + minuteAngle_;
            const auto minuteLength = 0.4 * radius_;
            const double minuteX = minuteLength * std::cos(minuteAngle * pi / 180);
            const double minuteY = minuteLength * std::sin(minuteAngle * pi / 180);
            const auto minuteEnd = center + QPoint(minuteX, minuteY);
            const auto minutePen = QPen { { minuteColor_ }, 20, Qt::SolidLine, Qt::RoundCap };
            painter.setBrush(Qt::NoBrush);
            painter.setPen(minutePen);
            painter.drawLine(center, minuteEnd);

            const auto hourAngle = 270 + hourAngle_;
            const auto hourLength = 0.3 * radius_;
            const double hourX = hourLength * std::cos(hourAngle * std::numbers::pi / 180);
            const double hourY = hourLength * std::sin(hourAngle * std::numbers::pi / 180);
            const auto hourEnd = center + QPoint(hourX, hourY);
            const auto hourPen = QPen { { hourColor_ }, 20, Qt::SolidLine, Qt::RoundCap };
            painter.setPen(hourPen);
            painter.drawLine(center, hourEnd);
        };

    private:
        uint32_t secondColor_;
        uint32_t minuteColor_;
        uint32_t hourColor_;

        double secondAngle_ = 100;
        double minuteAngle_ = 100;
        double hourAngle_ = 100;

        double radius_ = 100;
    };
}

class ScallopClock : public Extension<QWidget> {
    Q_OBJECT
public:
    explicit ScallopClock(QWidget* parent = nullptr)
        : Extension(parent) {
        waveCircle_.setFlange(12);
        waveCircle_.setFlangeRatio(0.9);
        waveCircle_.setFlangeRadius(20);
        waveCircle_.setLineWidth(2);
    }

    void setRadius(int radius) {
        waveCircle_.setRadius(radius);
        clockPointer_.setRadius(radius);
        waveCircle_.setFlangeRadius(20. * radius / 150);
        waveCircle_.setLineWidth(2. * radius / 150);
        setFixedSize(2 * radius, 2 * radius);
    }

    // TODO: 指针更新时把 WaveCircle 也一起更新了，不知道什么毛病
    // 多了约 2% 的CPU占用，这显然是不可容忍的
    void setAngle(double secondAngle, double minuteAngle, double hourAngle) {
        clockPointer_.setAngle(secondAngle, minuteAngle, hourAngle);
    }

    void reloadTheme() override { }

private:
    internal::ClockPointer clockPointer_ { this };
    WaveCircle waveCircle_ { this };
};

}
