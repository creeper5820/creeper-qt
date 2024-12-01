#pragma once

#include "creeper-qt/utility/pid.hh"
#include "creeper-qt/widget/wave-circle.hh"
#include "creeper-qt/widget/widget.hh"

#include <qdatetime.h>
#include <qpainter.h>
#include <qtimer.h>

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

            const auto center = QPointF(width() / 2., height() / 2.);

            auto painter = QPainter { this };
            painter.setOpacity(1);
            painter.setRenderHint(QPainter::Antialiasing);

            const auto secondAngle = 270 + secondAngle_;
            const auto secondLength = 0.6 * radius_;
            const auto secondX = secondLength * std::cos(secondAngle * pi / 180);
            const auto secondY = secondLength * std::sin(secondAngle * pi / 180);
            const auto secondEnd = center + QPointF(secondX, secondY);
            painter.setBrush({ secondColor_ });
            painter.setPen(Qt::NoPen);
            painter.drawEllipse(secondEnd, 10, 10);

            const auto minuteAngle = 270 + minuteAngle_;
            const auto minuteLength = 0.4 * radius_;
            const auto minuteX = minuteLength * std::cos(minuteAngle * pi / 180);
            const auto minuteY = minuteLength * std::sin(minuteAngle * pi / 180);
            const auto minuteEnd = center + QPointF(minuteX, minuteY);
            const auto minutePen = QPen { { minuteColor_ }, 20, Qt::SolidLine, Qt::RoundCap };
            painter.setBrush(Qt::NoBrush);
            painter.setPen(minutePen);
            painter.drawLine(center, minuteEnd);

            const auto hourAngle = 270 + hourAngle_;
            const auto hourLength = 0.3 * radius_;
            const auto hourX = hourLength * std::cos(hourAngle * std::numbers::pi / 180);
            const auto hourY = hourLength * std::sin(hourAngle * std::numbers::pi / 180);
            const auto hourEnd = center + QPointF(hourX, hourY);
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
        waveCircle_.setLineWidth(2);
        waveCircle_.setFlange(12);
        waveCircle_.setFlangeRatio(0.9);
        waveCircle_.setFlangeRadius(20);
        reloadTheme();
    }

    void setRadius(int radius) {
        waveCircle_.setRadius(radius)
            .setFlangeRadius(20. * radius / 150)
            .setLineWidth(2. * radius / 150);
        clockPointer_.setRadius(radius);
        setFixedSize(2 * radius, 2 * radius);
    }

    /// @bug: 指针更新时把 WaveCircle 也一起更新了，不知道什么毛病，多了约 2%
    /// 的CPU占用，这显然是不可容忍的
    /// @note: 2024-11-21，问题解决，怀疑是指针和circle互为兄弟关系(?)，
    /// 会一起更新，具体还不知道什么原因，在circle那加了个帧缓存就好了
    void setAngle(double secondAngle, double minuteAngle, double hourAngle) {
        clockPointer_.setAngle(secondAngle, minuteAngle, hourAngle);
    }

    void reloadTheme() override {
        waveCircle_.setBackground(Theme::color("primary050"));
        waveCircle_.setLineColor(Theme::color("primary050"));
    }

private:
    WaveCircle waveCircle_ { this };
    internal::ClockPointer clockPointer_ { this };
};

class AutoScallopClock : public ScallopClock {
    Q_OBJECT
public:
    explicit AutoScallopClock()
        : ScallopClock() {
        connect(&clockTimer_, &QTimer::timeout, [this] {
            const auto date = QDateTime::currentDateTime();
            seconds_ = date.time().second();
            minutes_ = date.time().minute();
            hours_ = date.time().hour();
        });

        connect(&animationTimer_, &QTimer::timeout, [this] {
            const double secondAngleTarget = seconds_ * 6;
            const double secondAngleError = angleDifference(secondAngleTarget, secondAngle_);
            if (std::abs(secondAngleError) < 0.1) return;

            const double minuteAngleTarget = minutes_ * 6;
            const double minuteAngleError = angleDifference(minuteAngleTarget, minuteAngle_);

            const double hourAngleTarget = hours_ * 30;
            const double hourAngleError = angleDifference(hourAngleTarget, hourAngle_);

            secondAngle_ = updateWithPidError(secondAngle_, secondAngleError, 0.1);
            minuteAngle_ = updateWithPidError(minuteAngle_, minuteAngleError, 0.1);
            hourAngle_ = updateWithPidError(hourAngle_, hourAngleError, 0.1);

            secondAngle_ = normalizeAngle(secondAngle_);
            minuteAngle_ = normalizeAngle(minuteAngle_);
            hourAngle_ = normalizeAngle(hourAngle_);

            this->setAngle(secondAngle_, minuteAngle_, hourAngle_);
        });

        const auto date = QDateTime::currentDateTime();
        seconds_ = date.time().second();
        minutes_ = date.time().minute();
        hours_ = date.time().hour();
        secondAngle_ = seconds_ * 6;
        minuteAngle_ = minutes_ * 6;
        hourAngle_ = hours_ * 30;
        this->setAngle(secondAngle_, minuteAngle_, hourAngle_);

        using namespace std::chrono_literals;
        clockTimer_.start(1s);
        animationTimer_.start(refreshIntervalMs_);

        this->setRadius(150);
    }

private:
    double angleDifference(double target, double current) {
        double diff = std::fmod(target - current + 540, 360) - 180;
        return diff;
    }

    double normalizeAngle(double angle) { return std::fmod(angle + 360, 360); }

private:
    QTimer animationTimer_ { this };
    QTimer clockTimer_ { this };

    uint32_t seconds_;
    uint32_t minutes_;
    uint32_t hours_;

    double secondAngle_;
    double minuteAngle_;
    double hourAngle_;
};

}
