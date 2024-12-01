#include <creeper-qt/module/scallop-clock.hh>
#include <creeper-qt/utility/pid.hh>
#include <creeper-qt/widget/label.hh>
#include <creeper-qt/widget/main-window.hh>
#include <creeper-qt/widget/wave-circle.hh>

#include <qapplication.h>
#include <qdatetime.h>
#include <qpainter.h>
#include <qtimer.h>

using namespace creeper;

class Clock : public MainWindow {
    Q_OBJECT
public:
    explicit Clock()
        : MainWindow() {
        connect(&clockTimer_, &QTimer::timeout, [this] {
            const auto date = QDateTime::currentDateTime();
            seconds_ = date.time().second();
            minutes_ = date.time().minute();
            hours_ = date.time().hour();
            label.setText(date.toString() + "\nscallop-clock");
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

            clock_.setAngle(secondAngle_, minuteAngle_, hourAngle_);
        });

        const auto date = QDateTime::currentDateTime();
        seconds_ = date.time().second();
        minutes_ = date.time().minute();
        hours_ = date.time().hour();
        secondAngle_ = seconds_ * 6;
        minuteAngle_ = minutes_ * 6;
        hourAngle_ = hours_ * 30;
        clock_.setAngle(secondAngle_, minuteAngle_, hourAngle_);
        label.setText(date.toString() + "\nscallop-clock");

        using namespace std::chrono_literals;
        clockTimer_.start(1s);
        animationTimer_.start(10ms);

        label.setFont({ "monospace", 8, QFont::Bold });
        label.setStyleSheet("color: #575757;");
        label.setMinimumWidth(350);
        label.setAlignment(Qt::AlignCenter);
        label.moveCenter();

        clock_.setRadius(150);

        auto vertical = new QVBoxLayout;
        vertical->setAlignment(Qt::AlignCenter);
        vertical->addLayout(clock_.horizontalWithSelf());
        vertical->addLayout(label.horizontalWithSelf());

        auto mainWidget = new QWidget;
        mainWidget->setLayout(vertical);
        this->setCentralWidget(mainWidget);

        this->setFixedSize(400, 500);
    }

private:
    double angleDifference(double target, double current) {
        double diff = fmod(target - current + 540, 360) - 180;
        return diff;
    }

    double normalizeAngle(double angle) { return fmod(angle + 360, 360); }

private:
    QTimer animationTimer_ { this };
    QTimer clockTimer_ { this };

    Label label { this };
    ScallopClock clock_ { this };

    uint32_t seconds_;
    uint32_t minutes_;
    uint32_t hours_;

    double secondAngle_;
    double minuteAngle_;
    double hourAngle_;
};

int main(int argc, char** argv) {
    auto app = new QApplication(argc, argv);
    Theme::setTheme("common-white");
    auto window = new Clock;
    window->moveCenter();
    window->show();
    return app->exec();
}

#include "clock.moc"