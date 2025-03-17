#include "creeper-qt/widget/basic-shape.hh"
#include "creeper-qt/widget/image.hh"
#include "creeper-qt/widget/widget.hh"
#include "top-area.hh"

#include <qapplication.h>
#include <qcolor.h>
#include <qdebug.h>

#include <creeper-qt/container.hh>
#include <creeper-qt/module.hh>
#include <creeper-qt/widget.hh>
#include <qwidget.h>

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

        auto row = Row::create()
                       .setAlignment(Qt::AlignCenter)
                       .add(firstVerticalLayout())
                       .add(secondVerticalBox())
                       .add(listWidget1)
                       .build();

        auto title = new Label { "遵循Google Material Design3 的Qt框架" };
        title->setAlignment(Qt::AlignTop);
        title->setWordWrap(true);
        title->setFont(QFont("Nowar Warcraft Sans CN", 8, QFont::Normal));

        auto top = Row::create()
                       .add(new TopLeftArea)
                       .add(new QuickAutoTheme<TopRightArea> {
                           [](auto& widget) { widget.setBackground(Theme::color("primary050")); } })
                       .build();

        auto column = Column::create()
                          .setAlignment(Qt::AlignTop)
                          .setMargin(10)
                          .setSpacing(10)
                          .add(top)
                          .add(row)
                          .build();

        auto mainWidget = new QWidget;
        mainWidget->setLayout(column);
        setCentralWidget(mainWidget);
    }

    QVBoxLayout* firstVerticalLayout() {
        const auto buttonFont = QFont("Nowar Warcraft Sans CN", 8, QFont::Normal);
        const auto buttonSize = QSize(100, 50);

        static auto buttons = std::array<PushButton*, 3> {};
        for (int index = 0; auto& button : buttons) {
            button = PushButton::create()
                         .setText("很好的按钮")
                         .setFixedSize(buttonSize)
                         .setFont(buttonFont)
                         .setRadiusRatio(0.2)
                         .build();
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
            Theme::setTheme(Theme::theme() == Theme::common::green //
                    ? Theme::common::purple
                    : Theme::common::green);
            Theme::reloadTheme();
        });

        auto switchButton3 = new ConvexSwitchButton;
        switchButton3->setFixedSize({ 60, 30 });

        auto switchButton1 = new ConcaveSwitchButton;
        switchButton1->setFixedWidth(80);

        auto column = Column::create()
                          .setAlignment(Qt::AlignTop | Qt::AlignCenter)
                          .setSpacing(10)
                          .add(rectangle)
                          .add(buttons[0])
                          .add(buttons[1])
                          .add(buttons[2])
                          .add(switchButton1)
                          .add(switchButton3)
                          .add(themeSwitchButton)
                          .build();

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

        return column;
    }

    QVBoxLayout* secondVerticalBox() {
        auto longSwitchButton = new ConvexSwitchButton;
        longSwitchButton->setFixedSize({ 200, 30 });

        auto slider0 = new ConcaveSlider;
        slider0->setFixedSize(200, 30);

        auto slider1 = new ConvexSlider;
        slider1->setFixedSize(200, 30);

        auto image = QuickAutoTheme<Image>::create(
            [](auto& image) { image.setBorderColor(Theme::color("primary200")); },
            ":/theme/icon/example/397017307286402.png")
                         .setFitness(Image::Fitness::Cover)
                         .setFixedSize({ 400, 175 })
                         .setBorderWidth(2)
                         .setRadius(10)
                         .build();

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

        /// @note 究级嵌套，只是觉得很好玩
        auto col = Column::create()
                       .setAlignment(Qt::AlignTop)
                       .add(image)
                       .add(switchCard1)
                       .add(Row::create()
                               .setMargin(0)
                               .add(Column::create()
                                       .setAlignment(Qt::AlignTop)
                                       .add(longSwitchButton)
                                       .add(slider0)
                                       .add(slider1)
                                       .add(lineEdit)
                                       .add(Row::create()
                                               .setAlignment(Qt::AlignLeft)
                                               .add(roundIconButton0)
                                               .add(roundIconButton1)
                                               .add(roundIconButton2)
                                               .build())
                                       .build())
                               .add(clock)
                               .build())
                       .build();

        return col;
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
    window.setFixedSize(1200, 700);
    window.moveCenter();

    window.show();

    return app.exec();
}

#include "widgets.moc"
