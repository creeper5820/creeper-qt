#include <qapplication.h>

#include "creeper-qt/widget/concave-slider.hh"
#include "creeper-qt/widget/convex-slider.hh"
#include "creeper-qt/widget/line-edit.hh"
#include "creeper-qt/widget/list-widget.hh"
#include "creeper-qt/widget/main-window.hh"
#include "creeper-qt/widget/menu.hh"
#include "creeper-qt/widget/push-button.hh"
#include "creeper-qt/widget/switch-button.hh"

#include "creeper-qt/module/round-icon-button.hh"
#include "creeper-qt/module/scallop-clock.hh"
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
        static auto buttons = std::array<PushButton*, 3> {};
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

        static auto menu = new Menu;
        menu->addAction("hello world");
        menu->addAction("hello world");
        menu->addAction("hello world");

        connect(buttons[0], &PushButton::released, [=] {
            const auto& button = buttons[0];
            const auto position = button->mapToGlobal(QPoint(0, button->height()));
            menu->exec(position);
            qDebug() << position;
        });

        return verticalLayout;
    }

    QVBoxLayout* secondVerticalBox() {
        auto longSwitchButton = new ConvexSwitchButton;
        longSwitchButton->setFixedSize({ 200, 30 });

        auto slider = new ConcaveSlider;
        slider->setFixedSize(200, 30);

        auto slider0 = new ConvexSlider;
        slider0->setFixedSize(200, 30);

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

        auto clock = new AutoScallopClock;
        clock->setRadius(120);

        auto roundIconButtonLayout = new QHBoxLayout;
        roundIconButtonLayout->setAlignment(Qt::AlignLeft);
        roundIconButtonLayout->addWidget(roundIconButton0);
        roundIconButtonLayout->addWidget(roundIconButton1);
        roundIconButtonLayout->addWidget(roundIconButton2);

        auto verticalLayout0 = new QVBoxLayout;
        verticalLayout0->setAlignment(Qt::AlignTop);
        verticalLayout0->addWidget(longSwitchButton);
        verticalLayout0->addWidget(slider);
        verticalLayout0->addWidget(slider0);
        verticalLayout0->addWidget(lineEdit);
        verticalLayout0->addLayout(roundIconButtonLayout);

        auto horizonLayout0 = new QHBoxLayout;
        horizonLayout0->addLayout(verticalLayout0);
        horizonLayout0->addWidget(clock);

        auto verticalLayout1 = new QVBoxLayout;
        verticalLayout1->setAlignment(Qt::AlignTop);
        verticalLayout1->setContentsMargins(10, 10, 10, 10);
        verticalLayout1->addWidget(switchCard0);
        verticalLayout1->addWidget(switchCard1);
        verticalLayout1->addLayout(horizonLayout0);

        return verticalLayout1;
    }
};

int main(int argc, char* argv[]) {
    qputenv("QT_SCALE_FACTOR", "1");
    auto app = QApplication { argc, argv };

    Theme::setTheme("common-green");

    auto window = Widgets {};
    QIcon icon { ":/theme/icon/normal/menu.png" };
    window.setWindowTitle("HelloWorld");
    window.setIconSize({ 10, 10 });
    window.setWindowIcon(icon);
    window.moveCenter();
    window.show();

    return app.exec();
}

#include "widgets.moc"