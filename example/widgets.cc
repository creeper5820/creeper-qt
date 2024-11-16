#include "qapplication.h"

#include "creeper-qt/widget/line-edit.hh"
#include "creeper-qt/widget/list-widget.hh"
#include "creeper-qt/widget/main-window.hh"
#include "creeper-qt/widget/push-button.hh"
#include "creeper-qt/widget/slider.hh"
#include "creeper-qt/widget/switch-button.hh"

#include "creeper-qt/module/round-icon-button.hh"
#include "creeper-qt/module/switch-card.hh"

using namespace creeper;

class Widgets : public creeper::MainWindow {
    Q_OBJECT
public:
    explicit Widgets()
        : MainWindow() {
        auto listWidget1 = new ListWidget;
        for (int i = 0; i < 8; i++)
            listWidget1->addSwitchAndLabel("你好世界 HelloWorld");

        auto horizonLayout = new QHBoxLayout;
        horizonLayout->setAlignment(Qt::AlignCenter);
        horizonLayout->addLayout(firstVerticalLayout());
        horizonLayout->addLayout(secondVerticalBox());
        horizonLayout->addWidget(listWidget1);

        auto mainWidget = new QWidget;
        mainWidget->setLayout(horizonLayout);
        setCentralWidget(mainWidget);
    }

    QVBoxLayout* firstVerticalLayout() {
        auto buttons = std::array<PushButton*, 3> {};
        for (int index = 0; auto& button : buttons) {
            button = new PushButton;
            button->setText("按钮" + QString::number(index++));
            button->setFixedSize({ 100, 50 });
            button->setFont(QFont("monospace", 12, QFont::Normal));
        }

        auto switchButton0 = new ConvexSwitchButton;
        switchButton0->setFixedSize({ 60, 30 });

        auto switchButton3 = new ConvexSwitchButton;
        switchButton3->setFixedSize({ 80, 40 });

        auto switchButton1 = new ConcaveSwitchButton;
        switchButton1->setFixedSize({ 80, 40 });

        auto verticalLayout = new QVBoxLayout;
        verticalLayout->setAlignment(Qt::AlignTop | Qt::AlignCenter);
        verticalLayout->setSpacing(10);
        verticalLayout->addWidget(buttons[0]);
        verticalLayout->addWidget(buttons[1]);
        verticalLayout->addWidget(buttons[2]);
        verticalLayout->addWidget(switchButton1);
        verticalLayout->addWidget(switchButton3);
        verticalLayout->addWidget(switchButton0);

        // test
        auto slider1 = new Slider;
        slider1->setLength(100);
        slider1->setThickness(5);
        slider1->setValue(0);
        verticalLayout->addWidget(slider1);

        auto slider2 = new Slider;
        slider2->setLength(200);
        slider2->setThickness(10);
        slider2->setValue(20);
        verticalLayout->addWidget(slider2);

        auto slider3 = new Slider;
        slider3->setLength(300);
        slider3->setThickness(20);
        slider3->setValue(40);
        slider3->setSliderRoundRatio(1.0);
        verticalLayout->addWidget(slider3);

        auto slider4 = new Slider;
        slider4->setLength(400);
        slider4->setThickness(30);
        slider4->setValue(60);
        slider4->setBLockThickness(60);
        verticalLayout->addWidget(slider4);

        auto slider5 = new Slider;
        slider5->setLength(500);
        slider5->setThickness(40);
        slider5->setValue(80);
        slider5->setSliderRoundRatio(1.0);
        verticalLayout->addWidget(slider5);

        auto slider6 = new Slider;
        slider6->setLength(600);
        slider6->setThickness(50);
        slider6->setBlockBorderShadowSize_(10);
        slider6->setBLockLength(100);
        slider6->setValue(100);

        verticalLayout->addWidget(slider6);

        return verticalLayout;
    }

    QVBoxLayout* secondVerticalBox() {
        auto longSwitchButton = new ConvexSwitchButton;
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

        return switchCardsLayout;
    }
};

int main(int argc, char* argv[]) {
    auto app = new QApplication { argc, argv };

    Theme::setTheme("common-white");

    auto window = new Widgets;
    auto icon = QIcon(":/theme/icon/normal/menu.png");
    window->setWindowIcon(icon);
    window->setIconSize({ 10, 10 });
    window->setWindowTitle("HelloWorld");
    window->moveCenter();
    window->show();

    return app->exec();
}

#include "widgets.moc"