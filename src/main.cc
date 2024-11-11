#include <qapplication.h>
#include <qlayout.h>
#include <qlayoutitem.h>

#include "widget/list-widget.hh"
#include "widget/main-window.hh"
#include "widget/push-button.hh"
#include "widget/switch-button.hh"

class MainWindowExample : public creeper::MainWindow {
    Q_OBJECT
public:
    explicit MainWindowExample()
        : MainWindow() {
        using namespace creeper;

        auto verticalLayout = new QVBoxLayout;
        verticalLayout->setAlignment(Qt::AlignTop);
        verticalLayout->setSpacing(10);
        auto buttons = std::array<PushButton*, 3> {};
        for (int index = 0; auto& button : buttons) {
            button = new creeper::PushButton;
            button->setText("Button" + QString::number(index++));
            button->setFixedSize({ 100, 50 });
            verticalLayout->addWidget(button);
        }

        auto switchButton0 = new SwitchButton;
        switchButton0->setFixedSize({ 60, 30 });
        verticalLayout->addWidget(switchButton0);

        auto switchButton3 = new SwitchButton;
        switchButton3->setFixedSize({ 60, 30 });
        verticalLayout->addWidget(switchButton3);

        auto switchButton1 = new SwitchButton;
        switchButton1->setFixedSize({ 60, 30 });
        verticalLayout->addWidget(switchButton1);

        auto horizonLayout = new QHBoxLayout;
        horizonLayout->setAlignment(Qt::AlignCenter);
        horizonLayout->addLayout(verticalLayout);

        auto listWidget1 = new ListWidget;
        horizonLayout->addWidget(listWidget1);
        listWidget1->addSwitchAndLabel("你好世界 HelloWorld");
        listWidget1->addSwitchAndLabel("你好世界 HelloWorld");
        listWidget1->addSwitchAndLabel("你好世界 HelloWorld");
        listWidget1->addSwitchAndLabel("你好世界 HelloWorld");

        auto listWidget0 = new ListWidget;
        horizonLayout->addWidget(listWidget0);
        listWidget0->addSimpleLabel("你好世界 HelloWorld");
        listWidget0->addSimpleLabel("你好世界 HelloWorld");
        listWidget0->addSimpleLabel("你好世界 HelloWorld");
        listWidget0->addSimpleLabel("你好世界 HelloWorld");
        listWidget0->addSimpleLabel("你好世界 HelloWorld");
        listWidget0->addSimpleLabel("你好世界 HelloWorld");

        auto mainWidget = new QWidget;
        mainWidget->setLayout(horizonLayout);
        setCentralWidget(mainWidget);
    }

private:
};

int main(int argc, char* argv[]) {
    auto app = new QApplication { argc, argv };

    creeper::Theme::setTheme("common-white");

    auto window = new MainWindowExample;
    window->show();

    return app->exec();
}

#include "main.moc"