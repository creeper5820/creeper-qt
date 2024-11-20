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
            if (seconds_ == 0) secondAngle_ = -6;
            if (minutes_ == 0) minuteAngle_ = -6;
            if (hours_ == 0) hourAngle_ = -30;
            label.setText(date.toString());
        });

        connect(&animationTimer_, &QTimer::timeout, [this] {
            secondAngle_ = updateWithPid(secondAngle_, static_cast<double>(seconds_ * 6), 0.1);
            minuteAngle_ = updateWithPid(minuteAngle_, static_cast<double>(minutes_ * 6), 0.1);
            hourAngle_ = updateWithPid(hourAngle_, static_cast<double>(hours_ * 30), 0.1);
            clock_.setAngle(secondAngle_, minuteAngle_, hourAngle_);
        });

        seconds_ = QDateTime::currentDateTime().time().second();
        secondAngle_ = seconds_ * 6;
        minutes_ = QDateTime::currentDateTime().time().minute();
        minuteAngle_ = minutes_ * 6;
        hours_ = QDateTime::currentDateTime().time().hour();
        hourAngle_ = hours_ * 30;

        using namespace std::chrono_literals;
        clockTimer_.start(1s);
        animationTimer_.start(10ms);

        label.setFont({ "monospace", 15 });
        label.setMinimumWidth(350);
        label.setAlignment(Qt::AlignCenter);
        label.moveCenter();

        clock_.setRadius(150);

        auto horizonLayout = new QHBoxLayout;
        horizonLayout->setAlignment(Qt::AlignCenter);
        horizonLayout->addWidget(&label);
        horizonLayout->addWidget(&clock_);

        auto mainWidget = new QWidget;
        mainWidget->setLayout(horizonLayout);
        this->setCentralWidget(mainWidget);
    }

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