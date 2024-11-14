#include <qapplication.h>
#include <qlayout.h>
#include <qlayoutitem.h>
#include <qlineedit.h>

#include "creeper-qt/widget/line-edit.hh"
#include "creeper-qt/widget/list-widget.hh"
#include "creeper-qt/widget/main-window.hh"
#include "creeper-qt/widget/push-button.hh"
#include "creeper-qt/widget/switch-button.hh"

#include "creeper-qt/module/round-icon-button.hh"
#include "creeper-qt/module/switch-card.hh"

class MainWindowExample : public creeper::MainWindow {
    Q_OBJECT
public:
    explicit MainWindowExample()
        : MainWindow() {
        using namespace creeper;

        auto buttons = std::array<PushButton*, 3> {};
        for (int index = 0; auto& button : buttons) {
            button = new PushButton;
            button->setText("按钮" + QString::number(index++));
            button->setFixedSize({ 100, 50 });
            button->setFont(QFont("monospace", 12, QFont::Normal));
        }

        auto switchButton0 = new SwitchButton;
        switchButton0->setFixedSize({ 60, 30 });

        auto switchButton3 = new SwitchButton;
        switchButton3->setFixedSize({ 60, 30 });

        auto switchButton1 = new SwitchButton;
        switchButton1->setFixedSize({ 60, 30 });

        auto longSwitchButton = new SwitchButton;
        longSwitchButton->setFixedSize({ 200, 30 });

        auto switchCard0 = new SwitchCard;
        switchCard0->setFixedSize({ 400, 175 });
        switchCard0->setText("将世界设定为“你好世界”");

        auto switchCard1 = new SwitchCard;
        switchCard1->setFixedSize({ 400, 125 });
        switchCard1->setText("将世界设定为“HELLO WORLD”");

        auto lineEdit = new LineEdit;
        lineEdit->setMaximumWidth(200);
        lineEdit->setIcon(QIcon(":/theme/icon/normal/search.png"));
        lineEdit->setPlaceholderText("HELLO WORLD");

        auto roundIconButton0 = new RoundIconButton;
        roundIconButton0->setRadius(20);
        roundIconButton0->setIconRatio(1.2);
        roundIconButton0->setIcon(QIcon(":/theme/icon/normal/home.png"));

        auto roundIconButton1 = new RoundIconButton;
        roundIconButton1->setRadius(20);
        roundIconButton1->setIconRatio(0.8);
        roundIconButton1->setIcon(QIcon(":/theme/icon/normal/return.png"));

        auto roundIconButton2 = new RoundIconButton;
        roundIconButton2->setRadius(20);
        roundIconButton2->setIconRatio(1);
        roundIconButton2->setIcon(QIcon(":/theme/icon/normal/menu.png"));

        auto listWidget1 = new ListWidget;
        listWidget1->addSwitchAndLabel("你好世界 HelloWorld");
        listWidget1->addSwitchAndLabel("你好世界 HelloWorld");
        listWidget1->addSwitchAndLabel("你好世界 HelloWorld");
        listWidget1->addSwitchAndLabel("你好世界 HelloWorld");

        auto verticalLayout = new QVBoxLayout;
        verticalLayout->setAlignment(Qt::AlignTop);
        verticalLayout->setSpacing(10);
        verticalLayout->addWidget(buttons[0]);
        verticalLayout->addWidget(buttons[1]);
        verticalLayout->addWidget(buttons[2]);
        verticalLayout->addWidget(switchButton0);
        verticalLayout->addWidget(switchButton3);
        verticalLayout->addWidget(switchButton1);

        auto roundIconButtonLayout = new QHBoxLayout;
        roundIconButtonLayout->setAlignment(Qt::AlignLeft);
        roundIconButtonLayout->addWidget(roundIconButton0);
        roundIconButtonLayout->addWidget(roundIconButton1);
        roundIconButtonLayout->addWidget(roundIconButton2);

        auto switchCardsLayout = new QVBoxLayout;
        switchCardsLayout->setAlignment(Qt::AlignTop);
        switchCardsLayout->setContentsMargins(10, 10, 10, 10);
        switchCardsLayout->addWidget(switchCard0);
        switchCardsLayout->addWidget(switchCard1);
        switchCardsLayout->addWidget(longSwitchButton);
        switchCardsLayout->addWidget(lineEdit);
        switchCardsLayout->addLayout(roundIconButtonLayout);

        auto horizonLayout = new QHBoxLayout;
        horizonLayout->setAlignment(Qt::AlignCenter);
        horizonLayout->addLayout(verticalLayout);
        horizonLayout->addLayout(switchCardsLayout);
        horizonLayout->addWidget(listWidget1);

        auto mainWidget = new QWidget;
        mainWidget->setLayout(horizonLayout);
        setCentralWidget(mainWidget);

        connect(switchButton0, &SwitchButton::released, this, [=] {
            qDebug() << "switch-button0: " << switchButton0->switched();
        });
    }

private:
};

int main(int argc, char* argv[]) {
    auto app = new QApplication { argc, argv };

    creeper::Theme::setTheme("common-white");

    auto window = new MainWindowExample;
    window->setWindowIcon(QIcon(":/theme/icon/normal/menu.png"));
    window->setIconSize({ 10, 10 });
    window->setWindowTitle("HelloWorld");
    window->moveCenter();
    window->show();

    return app->exec();
}

#include "main.moc"