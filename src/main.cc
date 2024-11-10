#include "qlayoutitem.h"
#include <qapplication.h>
#include <qlayout.h>

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
        verticalLayout->setSpacing(10);
        auto buttons = std::array<PushButton*, 5> {};
        for (int index = 0; auto& button : buttons) {
            button = new creeper::PushButton;
            button->setText("Button" + QString::number(index++));
            button->setFixedSize({ 100, 50 });
            verticalLayout->addWidget(button);
        }

        auto switchButton = new SwitchButton;
        switchButton->setFixedWidth(50);
        verticalLayout->addWidget(switchButton);

        auto horizontalLayout = new QHBoxLayout;
        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 5);
        horizontalLayout->addLayout(verticalLayout);

        auto mainWidget = new QWidget;
        mainWidget->setLayout(horizontalLayout);
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