#include "window.hh"
#include "video.hh"

#include <creeper-qt/widget/basic-shape.hh>
#include <creeper-qt/widget/label.hh>
#include <creeper-qt/widget/push-button.hh>
#include <creeper-qt/widget/slider.hh>
#include <creeper-qt/widget/switch-button.hh>

using namespace creeper;

struct Workbench::Impl {
    Video video;
    PushButton connectButton;
};

Workbench::Workbench(QWidget* parent)
    : pimpl_(new Impl)
    , MainWindow(parent) {

    // first column
    auto v0 = new QVBoxLayout;
    v0->addWidget(&pimpl_->video);

    auto g = new QGridLayout;

    auto label0 = new Label();
    label0->setText("分捡进度");
    label0->setFixedSize(100, 50);
    auto progress0 = new ConcaveSlider();
    progress0->setRange(0, 100);
    progress0->setFixedSize(200, 30);
    progress0->setValue(50);
    g->addWidget(label0, 0, 0, 1, 1);
    g->addWidget(progress0, 0, 1, 1, 1);

    auto label1 = new Label();
    label1->setText("剩余电量");
    label1->setFixedSize(100, 50);
    auto progress1 = new ConcaveSlider();
    progress1->setRange(0, 100);
    progress1->setFixedSize(200, 30);
    progress1->setValue(80);
    g->addWidget(label1, 1, 0, 1, 1);
    g->addWidget(progress1, 1, 1, 1, 1);

    auto label2 = new Label();
    label2->setText("货物储备");
    label2->setFixedSize(100, 50);
    auto progress2 = new ConcaveSlider();
    progress2->setRange(0, 100);
    progress2->setFixedSize(200, 30);
    progress2->setValue(30);
    g->addWidget(label2, 2, 0, 1, 1);
    g->addWidget(progress2, 2, 1, 1, 1);

    auto label00 = new Label();
    label00->setText("其他参数一");
    label00->setFixedSize(100, 50);
    auto progress00 = new ConcaveSlider();
    progress00->setRange(0, 100);
    progress00->setFixedSize(200, 30);
    progress00->setValue(100);
    g->addWidget(label00, 0, 2, 1, 1);
    g->addWidget(progress00, 0, 3, 1, 1);

    auto label01 = new Label();
    label01->setText("其他参数二");
    label01->setFixedSize(100, 50);
    auto progress01 = new ConcaveSlider();
    progress01->setRange(0, 100);
    progress01->setFixedSize(200, 30);
    progress01->setValue(40);
    g->addWidget(label01, 1, 2, 1, 1);
    g->addWidget(progress01, 1, 3, 1, 1);

    auto label02 = new Label();
    label02->setText("其他参数三");
    label02->setFixedSize(100, 50);
    auto progress02 = new ConcaveSlider();
    progress02->setRange(0, 100);
    progress02->setFixedSize(200, 30);
    progress02->setValue(70);
    g->addWidget(label02, 2, 2, 1, 1);
    g->addWidget(progress02, 2, 3, 1, 1);

    v0->addLayout(g);

    // second column
    auto v1 = new QVBoxLayout;
    v1->setAlignment(Qt::AlignTop);

    pimpl_->connectButton.setText("连接相机");
    pimpl_->connectButton.setFixedSize(120, 50);
    connect(&pimpl_->connectButton, &QPushButton::clicked, [this] {
        if (pimpl_->video.connected()) return;
        pimpl_->video.connectCamera(2);
    });
    v1->addWidget(&pimpl_->connectButton);

    auto startButton = new PushButton;
    startButton->setText("开启机器");
    startButton->setFixedSize(120, 50);
    v1->addWidget(startButton);

    auto stopButton = new PushButton;
    stopButton->setText("关闭机器");
    stopButton->setFixedSize(120, 50);
    v1->addWidget(stopButton);

    auto settingButton = new PushButton;
    settingButton->setText("设置");
    settingButton->setFixedSize(120, 50);
    v1->addWidget(settingButton);

    auto emergencyStopButton = new PushButton;
    emergencyStopButton->setText("紧急停止");
    emergencyStopButton->setFixedSize(120, 50);
    v1->addWidget(emergencyStopButton);

    auto errors = new RoundedRectangle;
    errors->setBorderColor(Theme::color("primary100"));
    errors->setBorderWidth(2);
    errors->setRadius(0);
    errors->setFixedWidth(120);
    errors->setMinimumHeight(240);
    v1->addWidget(errors);

    auto errorText = new Label;
    errorText->setAlignment(Qt::AlignCenter);
    auto vertical = new QVBoxLayout;
    vertical->addWidget(errorText);
    errors->setLayout(vertical);
    errorText->setText("错误信息\n一号：正常\n二号：正常\n"
                       "三号：正常\n四号：正常\n");

    auto horizon = new QHBoxLayout;
    horizon->setAlignment(Qt::AlignCenter);
    horizon->addLayout(v0);
    horizon->addLayout(v1);

    auto card = new RoundedRectangle;
    card->setBorderColor(Theme::color("primary100"));
    card->setBorderWidth(3);
    card->setRadius(5);
    card->setFixedSize(900, 600);
    card->setLayout(horizon);

    auto vertical0 = new QHBoxLayout;
    vertical0->addWidget(card);

    auto main = new QWidget;
    main->setLayout(vertical0);
    setCentralWidget(main);
}

Workbench::~Workbench() { delete pimpl_; }