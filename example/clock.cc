#include <creeper-qt/utility/pid.hh>
#include <creeper-qt/widget/label.hh>
#include <creeper-qt/widget/main-window.hh>
#include <creeper-qt/widget/wave-circle.hh>

#include <qapplication.h>
#include <qdatetime.h>
#include <qpainter.h>
#include <qtimer.h>

using namespace creeper;

class ClockWidget : public Extension<QWidget> {
    Q_OBJECT
public:
    explicit ClockWidget(QWidget* p = nullptr)
        : Extension(p) {
    }

    void setAngle(double _angle) {
        angle = _angle;
        update();
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        auto painter = QPainter { this };
        const auto theAngle = 270 + angle;
        const auto length = 0.4 * width();
        double x = length * std::cos(theAngle * std::numbers::pi / 180);
        double y = length * std::sin(theAngle * std::numbers::pi / 180);
        auto center = QPoint(width() / 2, height() / 2);
        auto end = center + QPoint(x, y);

        painter.setOpacity(1);
        painter.setRenderHint(QPainter::Antialiasing);

        painter.setPen(Qt::NoPen);
        painter.setBrush({ Theme::color("primary100") });
        painter.drawEllipse(QRect { 0, 0, width(), height() });

        painter.setBrush(Qt::NoBrush);
        painter.setPen(QPen { { Theme::color("primary600") }, 5, Qt::SolidLine, Qt::RoundCap });
        painter.drawLine(center, end);
    };

    void reloadTheme() override { }

private:
    double angle = 100;
};

class Clock : public MainWindow {
    Q_OBJECT
public:
    explicit Clock()
        : MainWindow() {
        connect(&clockTimer_, &QTimer::timeout, [this] {
            auto date = QDateTime::currentDateTime();
            label.setText(date.toString());
            seconds_++;
        });

        connect(&animationTimer_, &QTimer::timeout, [this] {
            static double angle = seconds_ * 6;
            angle = updateWithPid(angle, static_cast<double>(seconds_ * 6), 0.1);
            clock.setAngle(angle);
        });

        seconds_ = QDateTime::currentDateTime().time().second();

        using namespace std::chrono_literals;
        clockTimer_.start(1s);
        animationTimer_.start(10ms);

        label.setFont({ "monospace", 15 });
        label.setMinimumWidth(350);
        label.setAlignment(Qt::AlignCenter);
        label.moveCenter();

        clock.setFixedSize({ 200, 200 });

        waveCircle_.setRadius(150);
        waveCircle_.setFlange(6);

        auto horizonLayout = new QHBoxLayout;
        horizonLayout->setAlignment(Qt::AlignCenter);
        horizonLayout->addWidget(&label);
        horizonLayout->addWidget(&clock);
        horizonLayout->addWidget(&waveCircle_);

        auto mainWidget = new QWidget;
        mainWidget->setLayout(horizonLayout);
        this->setCentralWidget(mainWidget);
    }

private:
    QTimer animationTimer_ { this };
    QTimer clockTimer_ { this };

    Label label { this };
    ClockWidget clock { this };
    WaveCircle waveCircle_ { this };

    uint32_t seconds_;
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