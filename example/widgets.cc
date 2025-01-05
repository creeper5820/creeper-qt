#include <qapplication.h>
#include <qdebug.h>

#include "creeper-qt/widget/basic-shape.hh"
#include "creeper-qt/widget/combo-box.hh"
#include "creeper-qt/widget/concave-slider.hh"
#include "creeper-qt/widget/convex-slider.hh"
#include "creeper-qt/widget/image.hh"
#include "creeper-qt/widget/line-edit.hh"
#include "creeper-qt/widget/list-widget.hh"
#include "creeper-qt/widget/main-window.hh"
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
        listWidget1->setFont(QFont(Theme::font("text"), 8, QFont::Normal));
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
            button->setFont(QFont("Nowar Warcraft Sans CN", 8, QFont::Normal));
            button->setText("按钮" + QString::number(index++));
            button->setFixedSize({ 100, 50 });
        }

        auto rectangle = new QuickAutoTheme<RoundedRectangle> { [](auto& rect) {
            rect.setBackground(Theme::color("primary500"));
        } };
        rectangle->setFixedSize({ 100, 100 });

        /// @note 代码中并未显式调用update()，但加载主题时全部widget会进行一次更新,
        /// 这是怎么绘世呢？
        auto themeSwitchButton = new ConvexSwitchButton;
        themeSwitchButton->setFixedSize({ 60, 30 });
        connect(themeSwitchButton, &ConvexSwitchButton::clicked, [] {
            Theme::setTheme(Theme::theme() == "common-green" ? "common-purple" : "common-green");
            Theme::reloadTheme();
        });

        auto switchButton3 = new ConvexSwitchButton;
        switchButton3->setFixedSize({ 60, 30 });

        auto switchButton1 = new ConcaveSwitchButton;
        switchButton1->setFixedWidth(80);

        auto comboBox = new ComboBox;
        comboBox->setFixedSize({ 80, 40 });
        comboBox->addItem("hello world1");
        comboBox->addItem("hello world2");
        comboBox->addItem("hello world3");
        comboBox->addItem("hello world4");

        auto verticalLayout = new QVBoxLayout;
        verticalLayout->setAlignment(Qt::AlignTop | Qt::AlignCenter);
        verticalLayout->setSpacing(10);
        verticalLayout->addWidget(rectangle);
        verticalLayout->addWidget(buttons[0]);
        verticalLayout->addWidget(buttons[1]);
        verticalLayout->addWidget(buttons[2]);
        verticalLayout->addWidget(switchButton1);
        verticalLayout->addWidget(switchButton3);
        verticalLayout->addWidget(themeSwitchButton);

        buttons[0]->setText("切换主题");
        connect(buttons[0], &PushButton::released, [=] {
            static constexpr auto themes = std::array {
                Theme::common::blue,
                Theme::common::green,
                Theme::common::grey,
                Theme::common::purple,
            };
            static std::size_t index = 0;
            Theme::setTheme(themes[index++ % themes.size()]);
            Theme::reloadTheme();
        });

        return verticalLayout;
    }

    QVBoxLayout* secondVerticalBox() {
        auto longSwitchButton = new ConvexSwitchButton;
        longSwitchButton->setFixedSize({ 200, 30 });

        auto slider0 = new ConcaveSlider;
        slider0->setFixedSize(200, 30);

        auto slider1 = new ConvexSlider;
        slider1->setFixedSize(200, 30);

        auto image = new QuickAutoTheme<Image> {
            [](auto& image) { image.setBorderColor(Theme::color("primary200")); },
            ":/theme/icon/example/397017307286402.png"
        };
        image->setFitness(ImageFitness::Cover);
        image->setFixedSize({ 400, 175 });
        image->setBorderWidth(5);
        image->setRadius(10);

        auto switchCard1 = new SwitchCard;
        switchCard1->setFixedSize({ 400, 125 });
        switchCard1->setText("将世界设定为“HELLO WORLD”");

        auto lineEdit = new LineEdit;
        lineEdit->setMaximumWidth(200);
        lineEdit->setFixedHeight(40);
        lineEdit->setIcon(QIcon(":/theme/icon/normal/search.png"));
        lineEdit->setPlaceholderText("HELLO WORLD");
        lineEdit->setFont(QFont("monospace", 8, QFont::Normal));

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
        clock->setRadius(100);

        auto roundIconButtonLayout = new QHBoxLayout;
        roundIconButtonLayout->setAlignment(Qt::AlignLeft);
        roundIconButtonLayout->addWidget(roundIconButton0);
        roundIconButtonLayout->addWidget(roundIconButton1);
        roundIconButtonLayout->addWidget(roundIconButton2);

        auto verticalLayout0 = new QVBoxLayout;
        verticalLayout0->setAlignment(Qt::AlignTop);
        verticalLayout0->addWidget(longSwitchButton);
        verticalLayout0->addWidget(slider0);
        verticalLayout0->addWidget(slider1);
        verticalLayout0->addWidget(lineEdit);
        verticalLayout0->addLayout(roundIconButtonLayout);

        auto horizonLayout0 = new QHBoxLayout;
        horizonLayout0->setMargin(0);
        horizonLayout0->addLayout(verticalLayout0);
        horizonLayout0->addWidget(clock);

        auto verticalLayout1 = new QVBoxLayout;
        verticalLayout1->setAlignment(Qt::AlignTop);
        verticalLayout1->addWidget(image);
        verticalLayout1->addWidget(switchCard1);
        verticalLayout1->addLayout(horizonLayout0);

        return verticalLayout1;
    }
};

int main(int argc, char* argv[]) {
    // qputenv("QT_SCALE_FACTOR", "1");
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