#include <QtNetwork/QtNetwork>
#include <QtWidgets/QtWidgets>

#include <creeper-qt/container.hh>
#include <creeper-qt/widget.hh>

using namespace creeper;

class Keyboard : public MainWindow {
    Q_OBJECT
public:
    explicit Keyboard(QWidget* parent = nullptr)
        : MainWindow(parent) {
        auto main_widget = new QWidget {};

        auto slider = ConcaveSlider::create()
                          .setFixedSize(200, 30)
                          .setRange(1000, 2000)
                          .setValue(1500)
                          .build();

        main_widget->setLayout(Row::create() //
                .add(slider->verticalWithSelf())
                .build());

        setCentralWidget(main_widget);
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
    bool w_ { false };
    bool s_ { false };
    bool a_ { false };
    bool d_ { false };

    QUdpSocket socket_ { this };
    QTimer timer_;
};

int main(int argc, char* argv[]) {
    auto app = QApplication { argc, argv };
    Theme::setTheme(Theme::common::grey);

    auto window = Keyboard {};
    window.moveCenter();
    window.show();

    return app.exec();
}

#include "keyboard.moc"
