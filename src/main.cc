#include <qapplication.h>
#include <qlayout.h>
#include <qlayoutitem.h>

#include "widget/main-window.hh"
#include "widget/push-button.hh"
#include "widget/switch-button.hh"

class MainWindowExample : public creeper::MainWindow {
    Q_OBJECT
public:
    explicit MainWindowExample()
        : MainWindow() {
        buttonTest();
    }

    void buttonTest() {
        using namespace creeper;

        auto verticalLayout = new QVBoxLayout;
        verticalLayout->setAlignment(Qt::AlignCenter);
        verticalLayout->setSpacing(10);
        auto buttons = std::array<PushButton*, 3> {};
        for (int index = 0; auto& button : buttons) {
            button = new creeper::PushButton;
            button->setText("Button" + QString::number(index++));
            button->setFixedSize({ 100, 50 });
            verticalLayout->addWidget(button);
        }

        auto switchButton0 = new SwitchButton;
        switchButton0->setFixedSize({ 80, 30 });
        verticalLayout->addWidget(switchButton0);

        auto switchButton3 = new SwitchButton;
        switchButton3->setFixedSize({ 100, 60 });
        verticalLayout->addWidget(switchButton3);

        auto switchButton1 = new SwitchButton;
        switchButton1->setFixedSize({ 200, 60 });
        verticalLayout->addWidget(switchButton1);

        auto switchButton2 = new SwitchButton;
        switchButton2->setFixedSize({ 100, 30 });
        verticalLayout->addWidget(switchButton2);

        auto switchButton4 = new SwitchButton;
        switchButton4->setFixedSize({ 80, 30 });
        verticalLayout->addWidget(switchButton4);

        auto switchButton5 = new SwitchButton;
        switchButton5->setFixedSize({ 80, 10 });
        verticalLayout->addWidget(switchButton5);

        auto mainWidget = new QWidget;
        mainWidget->setLayout(verticalLayout);
        setCentralWidget(mainWidget);
    }

private:
};

int main(int argc, char* argv[]) {
    auto app = new QApplication { argc, argv };
    auto window = new MainWindowExample;
    window->show();
    return app->exec();
}

#include "main.moc"