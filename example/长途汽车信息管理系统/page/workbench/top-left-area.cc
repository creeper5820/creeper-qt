#include "top-area.hh"

#include <creeper-qt/module/round-icon-button.hh>
#include <creeper-qt/widget/line-edit.hh>
#include <creeper-qt/widget/push-button.hh>

using namespace creeper;

struct TopLeftArea::Impl {
    Impl() {
        main.setRadius(15);
        main.setIconRatio(1);
        main.setIcon(QIcon(":/image/google.png"));

        const auto buttonFont = QFont("Nowar Warcraft Sans CN", 8);
        const auto buttonSize = QSize(45, 30);

        PushButton::Style {
            .text = "文件",
            .size = buttonSize,
            .font = buttonFont,
            .background = false,
        }(file);
        PushButton::Style {
            .text = "编辑",
            .size = buttonSize,
            .font = buttonFont,
            .background = false,
        }(edit);
        PushButton::Style {
            .text = "查看",
            .size = buttonSize,
            .font = buttonFont,
            .background = false,
        }(view);
        PushButton::Style {
            .text = "数据",
            .size = buttonSize,
            .font = buttonFont,
            .background = false,
        }(data);
        PushButton::Style {
            .text = "帮助",
            .size = buttonSize,
            .font = buttonFont,
            .background = false,
        }(help);

        name.setPlaceholderText("长途汽车信息管理系统");
        name.setFont(QFont("Nowar Warcraft Sans CN", 8));
        name.setFixedSize(200, 30);
    };

    RoundIconButton main;
    LineEdit name;
    PushButton file;
    PushButton edit;
    PushButton view;
    PushButton data;
    PushButton help;
};

TopLeftArea::TopLeftArea(QWidget* parent)
    : RoundedRectangle(parent)
    , pimpl_(new Impl) {

    auto left = new QGridLayout;
    left->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    left->setMargin(5);
    left->setVerticalSpacing(5);
    left->setHorizontalSpacing(2);

    left->addLayout(pimpl_->main.horizontalWithSelf(), 0, 0, 1, 1);
    left->addWidget(&pimpl_->name, 0, 1, 1, 4);
    left->addWidget(&pimpl_->file, 1, 0, 1, 1);
    left->addWidget(&pimpl_->edit, 1, 1, 1, 1);
    left->addWidget(&pimpl_->view, 1, 2, 1, 1);
    left->addWidget(&pimpl_->data, 1, 3, 1, 1);
    left->addWidget(&pimpl_->help, 1, 4, 1, 1);

    auto right = new QHBoxLayout;
    right->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    right->setSpacing(10);
    right->setContentsMargins(5, 5, 5, 5);

    setLayout(left);
    setBackground(Theme::color("background"));
    setMaximumHeight(70);
}

TopLeftArea::~TopLeftArea() { delete pimpl_; }

void TopLeftArea::setFileName(const QString& name) { pimpl_->name.setText(name); }