#include "top-area.hh"

#include <creeper-qt/module/round-icon-button.hh>
#include <creeper-qt/setting/color.hh>
#include <creeper-qt/widget/image.hh>
#include <creeper-qt/widget/push-button.hh>
#include <qboxlayout.h>

using namespace creeper;

struct TopRightArea::Impl {
    QuickAutoTheme<Image> avatar { [](auto& image) {
        image.setBorderColor(Theme::color("primary300"));
    } };

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
        .pixmap = QPixmap(":/theme/icon/example/ohto-ai.png"),
        .size = QSize(50, 50),
        .radius = 25,
        .borderWidth = 3,
        .fitness = ImageFitness::Cover,
    }(pimpl_->avatar);

    PushButton::Style {
        .text = "MIKU",
        .size = QSize(50, 30),
        .font = font,
        .background = false,
    }(pimpl_->userSettings);

    pimpl_->button0.setIcon(QIcon(":/theme/icon/normal/github.png"));
    pimpl_->button0.setRadius(15);

    pimpl_->button1.setIcon(QIcon(":/theme/icon/normal/google.png"));
    pimpl_->button1.setRadius(15);

    pimpl_->button2.setIcon(QIcon(":/theme/icon/normal/school.png"));
    pimpl_->button2.setRadius(15);

    auto horizon = new QHBoxLayout;
    horizon->setAlignment(Qt::AlignRight);
    horizon->setSpacing(5);
    horizon->addWidget(&pimpl_->button0);
    horizon->addWidget(&pimpl_->button1);
    horizon->addWidget(&pimpl_->button2);
    horizon->addSpacing(20);
    horizon->addWidget(&pimpl_->userSettings);
    horizon->addWidget(&pimpl_->avatar);

    setLayout(horizon);
    setRadius(10);
    setBackground(color::grey100);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
}

TopRightArea::~TopRightArea() { delete pimpl_; }