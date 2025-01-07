#include "top-area.hh"

#include <creeper-qt/module/round-icon-button.hh>
#include <creeper-qt/setting/color.hh>
#include <creeper-qt/widget/image.hh>
#include <creeper-qt/widget/push-button.hh>
#include <qboxlayout.h>

using namespace creeper;

struct TopRightArea::Impl {
    Image avatar;
    PushButton userSettings;
    RoundIconButton button0;
    RoundIconButton button1;
    RoundIconButton button2;
};

TopRightArea::TopRightArea(QWidget* parent)
    : RoundedRectangle(parent)
    , pimpl_(new Impl) {

    const auto font = QFont("Nowar Warcraft Sans CN", 8);

    Image::Style {
        .pixmap = QPixmap(":/image/ohto-ai.png"),
        .size = QSize(50, 50),
        .radius = 25,
        .borderWidth = 3,
        .borderColor = Theme::color("primary200"),
        .fitness = ImageFitness::Cover,
    }(pimpl_->avatar);

    PushButton::Style {
        .text = "用户设置",
        .size = QSize(80, 30),
        .font = font,
        .borderColor = color::grey400,
        .backgroundColor = color::grey100,
        .borderWidth = 1,
        .autoTheme = false,
    }(pimpl_->userSettings);

    pimpl_->button0.setIcon(QIcon(":/image/google.png"));
    pimpl_->button0.setRadius(15);

    pimpl_->button1.setIcon(QIcon(":/image/github.png"));
    pimpl_->button1.setRadius(15);

    pimpl_->button2.setIcon(QIcon(":/image/school.png"));
    pimpl_->button2.setRadius(15);

    auto horizon = new QHBoxLayout;
    horizon->setAlignment(Qt::AlignRight);
    horizon->setSpacing(5);
    horizon->addWidget(&pimpl_->button0);
    horizon->addWidget(&pimpl_->button1);
    horizon->addWidget(&pimpl_->button2);
    horizon->addSpacing(60);
    horizon->addWidget(&pimpl_->userSettings);
    horizon->addWidget(&pimpl_->avatar);

    setLayout(horizon);
    setRadius(10);
    setBackground(color::grey100);
    setFixedWidth(340);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

TopRightArea::~TopRightArea() { delete pimpl_; }