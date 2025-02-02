#include "top-area.hh"

#include <creeper-qt/container.hh>
#include <creeper-qt/module/round-icon-button.hh>
#include <creeper-qt/setting/color.hh>
#include <creeper-qt/widget/image.hh>
#include <creeper-qt/widget/push-button.hh>
#include <qboxlayout.h>

using namespace creeper;

struct TopRightArea::Impl {
    PushButton* userSettings;
    RoundIconButton button0;
    RoundIconButton button1;
    RoundIconButton button2;
};

TopRightArea::TopRightArea(QWidget* parent)
    : RoundedRectangle(parent)
    , pimpl_(new Impl) {

    const auto font = QFont("Nowar Warcraft Sans CN", 8);

    pimpl_->userSettings = PushButton::create()
                               .setText("大户爱")
                               .setFixedSize(50, 30)
                               .setFont(font)
                               .disableBackground()
                               .build();

    pimpl_->button0.setIcon(QIcon(":/theme/icon/normal/github.png"));
    pimpl_->button0.setRadius(15);

    pimpl_->button1.setIcon(QIcon(":/theme/icon/normal/google.png"));
    pimpl_->button1.setRadius(15);

    pimpl_->button2.setIcon(QIcon(":/theme/icon/normal/school.png"));
    pimpl_->button2.setRadius(15);

    auto row = Row::create()
                   .setAlignment(Qt::AlignRight)
                   .setSpacing(5)
                   .add(&pimpl_->button0)
                   .add(&pimpl_->button1)
                   .add(&pimpl_->button2)
                   .addSpacing(20)
                   .add(pimpl_->userSettings)
                   .add(QuickAutoTheme<Image>::create([](auto& image) { //
                       image.setBorderColor(Theme::color("primary300"));
                   })
                           .setPixmap(QPixmap(":/theme/icon/example/ohto-ai.png"))
                           .setRadius(25)
                           .setBorderWidth(3)
                           .setFitness(Image::Fitness::Cover)
                           .setFixedSize(QSize(50, 50))
                           .build())
                   .build();

    setLayout(row);
    setRadius(10);
    setBackground(color::grey100);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
}

TopRightArea::~TopRightArea() { delete pimpl_; }