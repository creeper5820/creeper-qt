#include <QtNetwork/QtNetwork>
#include <qevent.h>

#include <creeper-qt/widget/label.hh>
#include <creeper-qt/widget/main-window.hh>

using namespace creeper;

class Keyboard : public MainWindow {
    Q_OBJECT
public:
    enum class Motor : bool { Left, Right };

    explicit Keyboard(QWidget* parent = nullptr)
        : MainWindow(parent) {
        connect(&timer_, &QTimer::timeout, [this] {
            move(w_ - s_, a_ - d_);
            socket_.write(reinterpret_cast<const char*>(data_), 8);
            label_.setText(
                QString("w:%1 s:%2 a:%3 d:%4").arg(w_).arg(s_).arg(a_).arg(d_));
        });

        label_.setFont({ "monospace", 15, QFont::Bold });
        label_.setMinimumWidth(200);
        label_.moveCenter();

        socket_.connectToHost("10.31.2.15", 8000);
        timer_.start(50);
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
    template <typename T> inline T clamp(T value, T left, T right) {
        value = value > right ? right : value;
        value = value < left ? left : value;
        return value;
    }

    template <Motor motor> inline void rotate(double speed) {
        speed = clamp(speed, -1.0, 1.0);
        int pwm1 = speed > 0 ? 255 * +speed : 0;
        int pwm2 = speed < 0 ? 255 * -speed : 0;
        if constexpr (motor == Motor::Left) {
            data_[0] = pwm1;
            data_[1] = pwm2;
        } else if constexpr (motor == Motor::Right) {
            data_[4] = pwm1;
            data_[5] = pwm2;
        }
    }

    inline void move(double v, double w) {
        v = clamp(v, -1., 1.);
        w = clamp(w, -1., 1.);
        rotate<Motor::Left>(v - w * 2);
        rotate<Motor::Right>(v + w * 2);
    }

private:
    QUdpSocket socket_ { this };
    Label label_ { this };
    QTimer timer_;
    uint8_t data_[9];
    bool w_ { false }, s_ { false }, a_ { false }, d_ { false };
};

int main(int argc, char* argv[]) {
    auto app = new QApplication { argc, argv };

    Theme::setTheme("common-white");

    auto window = new Keyboard;
    window->moveCenter();
    window->show();

    return app->exec();
}

#include "keyboard.moc"