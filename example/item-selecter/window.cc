#include "window.hh"

#include <creeper-qt/widget/label.hh>
#include <creeper-qt/widget/list-widget.hh>
#include <creeper-qt/widget/push-button.hh>
#include <creeper-qt/widget/slider.hh>

using namespace creeper;

struct Window::Impl {
    ListWidget leftVerticalList0;
    ListWidget leftVerticalList1;
    ListWidget rightVerticalList0;
    ListWidget rightVerticalList1;
    std::vector<QString> leftData;
    std::vector<QString> rightData;

    PushButton singleLeft2Right;
    PushButton singleRight2Left;
    PushButton allLeft2Right;
    PushButton allRight2Left;

    ConcaveSwitchButton switchTheme;
    Label switchThemeLabel;
    ConcaveSwitchButton switchShowMethod;
    Label switchShowMethodLabel;

    void generateExamples() {
        leftData.emplace_back("BeiJing");
        leftData.emplace_back("ShangHai");
        leftData.emplace_back("JiangSu");
        leftData.emplace_back("JiangXi");
        leftData.emplace_back("WuHang");
        leftData.emplace_back("XiAn");
        leftData.emplace_back("ZheJiang");
        leftData.emplace_back("NanJing");
        leftData.emplace_back("LanZhou");
    }

    void updateListSingleColumn() {
        leftVerticalList0.clear();
        for (const auto city : leftData)
            leftVerticalList0.addSimpleLabel(city);
        rightVerticalList0.clear();
        for (const auto city : rightData)
            rightVerticalList0.addSimpleLabel(city);
    }

    void updateListDoubleColumn() { }
};

Window::Window()
    : MainWindow()
    , pimpl_(new Impl) {
    setFont(QFont { "monospace", 15, QFont::Bold });

    auto h0 = new QHBoxLayout;
    h0->setAlignment(Qt::AlignCenter);
    h0->addWidget(&pimpl_->switchTheme);
    h0->addWidget(&pimpl_->switchThemeLabel);
    h0->addWidget(&pimpl_->switchShowMethod);
    h0->addWidget(&pimpl_->switchShowMethodLabel);

    auto g0 = new QGridLayout;
    g0->addWidget(&pimpl_->leftVerticalList0, 0, 0, 4, 2);
    g0->addWidget(&pimpl_->rightVerticalList0, 0, 3, 4, 2);
    g0->addWidget(&pimpl_->singleLeft2Right, 0, 2, 1, 1);
    g0->addWidget(&pimpl_->allLeft2Right, 1, 2, 1, 1);
    g0->addWidget(&pimpl_->singleRight2Left, 2, 2, 1, 1);
    g0->addWidget(&pimpl_->allRight2Left, 3, 2, 1, 1);

    auto v0 = new QVBoxLayout;
    v0->addLayout(h0);
    v0->addLayout(g0);

    pimpl_->switchShowMethodLabel.setText("switch show style");
    pimpl_->switchShowMethodLabel.setFixedSize(200, 50);
    pimpl_->switchShowMethodLabel.setFont(font());
    pimpl_->switchThemeLabel.setText("switch theme");
    pimpl_->switchThemeLabel.setFixedSize(200, 50);
    pimpl_->switchThemeLabel.setFont(font());

    pimpl_->switchTheme.setFixedSize({ 100, 50 });
    pimpl_->switchShowMethod.setFixedSize({ 100, 50 });

    pimpl_->singleLeft2Right.setText(">");
    pimpl_->singleLeft2Right.setFixedSize(100, 50);
    pimpl_->allLeft2Right.setText(">>");
    pimpl_->allLeft2Right.setFixedSize(100, 50);
    pimpl_->singleRight2Left.setText("<");
    pimpl_->singleRight2Left.setFixedSize(100, 50);
    pimpl_->allRight2Left.setText("<<");
    pimpl_->allRight2Left.setFixedSize(100, 50);

    const auto itemFont = QFont { "monospace", 12 };
    pimpl_->leftVerticalList0.setMinimumHeight(300);
    pimpl_->leftVerticalList0.setFont(itemFont);
    pimpl_->leftVerticalList1.setMinimumHeight(300);
    pimpl_->leftVerticalList1.setFont(itemFont);
    pimpl_->rightVerticalList0.setMinimumHeight(300);
    pimpl_->rightVerticalList0.setFont(itemFont);
    pimpl_->rightVerticalList1.setMinimumHeight(300);
    pimpl_->rightVerticalList1.setFont(itemFont);

    connect(&pimpl_->switchTheme, &ConcaveSwitchButton::released, [] {
        const auto theme = Theme::theme() == "common-green" ? "common-white" : "common-green";
        Theme::setTheme(theme);
        Theme::reloadTheme();
    });

    connect(&pimpl_->singleLeft2Right, &PushButton::released, [this] {
        const auto selected = pimpl_->leftVerticalList0.currentIndex().row();
        const auto item = pimpl_->leftVerticalList0.currentItem()->text();
        if (selected == -1) return;

        pimpl_->leftData.erase(pimpl_->leftData.begin() + selected);
        pimpl_->rightData.push_back(item);
        pimpl_->updateListSingleColumn();
    });

    connect(&pimpl_->allLeft2Right, &PushButton::released, [this] { });
    connect(&pimpl_->singleRight2Left, &PushButton::released, [this] { });
    connect(&pimpl_->allRight2Left, &PushButton::released, [this] { });

    auto main = new QWidget;
    main->setLayout(v0);
    setCentralWidget(main);

    pimpl_->generateExamples();
    pimpl_->updateListSingleColumn();
}

Window::~Window() { delete pimpl_; }