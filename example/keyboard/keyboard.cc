#include <QtNetwork/QtNetwork>
#include <QtWidgets/QtWidgets>

#include <creeper-qt/widget/label.hh>
#include <creeper-qt/widget/line-edit.hh>
#include <creeper-qt/widget/main-window.hh>
#include <creeper-qt/widget/push-button.hh>
#include <creeper-qt/widget/slider.hh>

using namespace creeper;

class Keyboard : public MainWindow {
    Q_OBJECT
public:
    enum class PinMode : uint8_t { None, Analog, Servo };
    struct __attribute__((packed)) RemoteControlCommand {
        uint8_t header = 0xa5;
        uint8_t values[10];
        PinMode modes[10];
        uint8_t end = 0xb6;
    };

    struct __attribute__((packed)) KeyboardCommand {
        uint8_t header = 0xa5;
        uint8_t values[10];
        PinMode modes[10];
        uint8_t end = 0xb6;
    };

    enum class Motor : bool { Left, Right };

    explicit Keyboard(QWidget* parent = nullptr)
        : MainWindow(parent) {
        connect(&timer_, &QTimer::timeout, [this] {
            command_.values[currentPinIndex_] += w_ * 5;
            command_.values[currentPinIndex_] -= s_ * 5;
            label_.setText(QString("w:%1 s:%2 a:%3 d:%4\nindex: %5 angle: %6")
                    .arg(w_)
                    .arg(s_)
                    .arg(a_)
                    .arg(d_)
                    .arg(currentPinIndex_)
                    .arg(command_.values[currentPinIndex_]));

            auto msg = QString("%1 %2 %3 %4").arg(w_).arg(s_).arg(a_).arg(d_);
            if (currentPinIndex_ != -1) socket_.write(msg.toUtf8());
        });

        label_.setFont({ "monospace", 15, QFont::Bold });
        label_.setAlignment(Qt::AlignCenter);
        label_.setMinimumWidth(200);
        label_.setMinimumHeight(100);

        slider_.setFixedWidth(200);
        slider_.setFixedHeight(30);
        slider_.setRange(0, 180);
        slider_.setValue(0);
        connect(&slider_, &ConcaveSlider::sliderMoved, [this](int position) {
            if (currentPinIndex_ == -1) return;
            command_.values[currentPinIndex_] = slider_.value();
        });

        buttonChangeMode_.setText("Unkown Mode");
        buttonChangeMode_.setFixedSize({ 200, 50 });
        connect(&buttonChangeMode_, &QPushButton::clicked, [this] {
            if (currentPinIndex_ == -1) return;
            auto& mode = command_.modes[currentPinIndex_];
            if (mode == PinMode::Servo) mode = PinMode::Analog;
            else if (mode == PinMode::Analog) mode = PinMode::Servo;
            else mode = PinMode::Servo;

            updateComponents();
        });

        pinSelectorEdit_.setFont({ "monospace", 15, QFont::Bold });
        pinSelectorEdit_.setAlignment(Qt::AlignCenter);
        pinSelectorEdit_.setFixedWidth(200);
        pinSelectorEdit_.setFixedHeight(50);
        connect(&pinSelectorEdit_, &LineEdit::textChanged, [this] {
            const auto text = pinSelectorEdit_.text();
            bool ok;
            int value = text.toInt(&ok);
            if (!ok || value > 9) return;
            currentPinIndex_ = value;
            updateComponents();
        });

        auto horizonLayout = new QHBoxLayout;
        horizonLayout->setAlignment(Qt::AlignCenter);
        horizonLayout->addWidget(&buttonChangeMode_);
        horizonLayout->addWidget(&pinSelectorEdit_);

        auto verticalLayout = new QVBoxLayout;
        verticalLayout->setAlignment(Qt::AlignCenter);
        verticalLayout->addWidget(&label_);

        auto horizonLayoutForSlider = new QHBoxLayout;
        horizonLayoutForSlider->setAlignment(Qt::AlignCenter);
        horizonLayoutForSlider->addWidget(&slider_);
        verticalLayout->addLayout(horizonLayoutForSlider);

        verticalLayout->addLayout(horizonLayout);

        auto mainWidget = new QWidget;
        mainWidget->setLayout(verticalLayout);
        setCentralWidget(mainWidget);

        socket_.connectToHost("116.62.240.46", 18954);
        timer_.start(100);
    }

protected:
    void keyPressEvent(QKeyEvent* event) override {
        if (event->key() == Qt::Key_W) w_ = true;
        if (event->key() == Qt::Key_S) s_ = true;
        if (event->key() == Qt::Key_A) a_ = true;
        if (event->key() == Qt::Key_D) d_ = true;
    }

    void keyReleaseEvent(QKeyEvent* event) override {
        if (event->key() == Qt::Key_W) w_ = false;
        if (event->key() == Qt::Key_S) s_ = false;
        if (event->key() == Qt::Key_A) a_ = false;
        if (event->key() == Qt::Key_D) d_ = false;
    }

private:
    void updateComponents() {
        const auto mode = command_.modes[currentPinIndex_];
        const auto value = command_.values[currentPinIndex_];
        slider_.setValue(value);
        if (mode == PinMode::Servo) {
            buttonChangeMode_.setText("Servo Mode");
        } else if (mode == PinMode::Analog) {
            buttonChangeMode_.setText("Analog Mode");
        } else {
            buttonChangeMode_.setText("Unkown Mode");
        }
    }

    template <typename T> inline T clamp(T value, T left, T right) {
        value = value > right ? right : value;
        value = value < left ? left : value;
        return value;
    }

    template <Motor motor> inline void rotate(double speed) {
        speed = clamp(speed, -1.0, 1.0);
        int pwm1 = speed > 0 ? 255 * +speed : 0;
        int pwm2 = speed < 0 ? 255 * -speed : 0;
    }

    inline void move(double v, double w) {
        v = clamp(v, -1., 1.);
        w = clamp(w, -1., 1.);
        rotate<Motor::Left>(v - w * 2);
        rotate<Motor::Right>(v + w * 2);
    }

private:
    Label label_;
    PushButton buttonChangeMode_;
    LineEdit pinSelectorEdit_;
    ConcaveSlider slider_;

    RemoteControlCommand command_;
    QUdpSocket socket_ { this };
    QTimer timer_;

    int currentPinIndex_ = -1;
    bool w_ { false }, s_ { false }, a_ { false }, d_ { false };
};

int main(int argc, char* argv[]) {
    auto app = new QApplication { argc, argv };

    Theme::setTheme("common-green");

    auto window = new Keyboard;
    window->moveCenter();
    window->show();

    return app->exec();
}

#include "keyboard.moc"