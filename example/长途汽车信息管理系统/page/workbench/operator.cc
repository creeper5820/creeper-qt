#include "operator.hh"

#include <creeper-qt/module/round-icon-button.hh>
#include <creeper-qt/setting/color.hh>
#include <creeper-qt/widget/push-button.hh>

using namespace creeper;

static inline constexpr auto clickedButtonColor = color::grey600;
static inline constexpr auto clickedTextColor = color::white;
static inline constexpr auto unclickedButtonColor = color::grey200;
static inline constexpr auto unclickedTextColor = color::grey900;

struct OperatorArea::Impl {
    std::array<PushButton, 4> functions;
    PushButton logout;
    RoundIconButton add;
    RoundIconButton move;
    RoundIconButton back;

    void updateFunctionButton(std::size_t clicked) {
        for (std::size_t i = 0; i < functions.size(); i++) {
            if (i == clicked) {
                functions[i].setBackgroundColor(clickedButtonColor);
                functions[i].setTextColor(clickedTextColor);
            } else {
                functions[i].setBackgroundColor(unclickedButtonColor);
                functions[i].setTextColor(unclickedTextColor);
            }
        }
    }

    void functionPlaceHolder() { }
};

OperatorArea::OperatorArea(QWidget* parent)
    : RoundedRectangle(parent)
    , pimpl_(new Impl) {

    auto basicStyle = PushButton::Style {};
    basicStyle.autoTheme = false;
    basicStyle.size = QSize(100, 40);
    basicStyle.text = "DefaultText";
    basicStyle.font = QFont("Nowar Warcraft Sans CN", 8);
    basicStyle.radiusRatio = 0.5;
    basicStyle.backgroundColor = unclickedButtonColor;
    basicStyle.textColor = unclickedTextColor;

    basicStyle.text = "仪表界面";
    basicStyle(pimpl_->functions[0]);

    basicStyle.text = "数据管理";
    basicStyle(pimpl_->functions[1]);

    basicStyle.text = "路线查看";
    basicStyle(pimpl_->functions[2]);

    basicStyle.text = "关于我们";
    basicStyle(pimpl_->functions[3]);

    auto& search = pimpl_->add;
    search.setRadius(15);
    search.setIcon(QIcon(":/theme/icon/normal/search.png"));
    auto& home = pimpl_->move;
    home.setRadius(15);
    home.setIconRatio(1.3);
    home.setIcon(QIcon(":/theme/icon/normal/home.png"));
    auto& back = pimpl_->back;
    back.setRadius(15);
    back.setIcon(QIcon(":/theme/icon/normal/return.png"));

    auto topLayout = new QHBoxLayout;
    topLayout->setAlignment(Qt::AlignCenter);
    topLayout->addWidget(&back);
    topLayout->addWidget(&search);
    topLayout->addWidget(&home);

    basicStyle.text = "Logout";
    basicStyle(pimpl_->logout);
    auto bottomLayout = new QVBoxLayout;
    bottomLayout->setAlignment(Qt::AlignBottom);
    bottomLayout->addWidget(&pimpl_->logout);

    auto verticalLayout = new QVBoxLayout;
    verticalLayout->setAlignment(Qt::AlignTop);
    verticalLayout->setMargin(10);
    verticalLayout->addLayout(topLayout, 0);
    verticalLayout->addSpacing(20);
    for (auto& button : pimpl_->functions)
        verticalLayout->addWidget(&button, 1);
    verticalLayout->addLayout(bottomLayout, 1);

    for (std::size_t i = 0; i < pimpl_->functions.size(); i++)
        connect(&pimpl_->functions[i], &PushButton::clicked, [this, i] {
            pimpl_->updateFunctionButton(i);
            emit changeView(i);
        });

    setLayout(verticalLayout);
    setBackground(color::grey100);
    setBorderColor(color::grey100);
}

OperatorArea::~OperatorArea() { delete pimpl_; }
