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

    bool singleColumn = true;

    void generateExamples() {
        leftData.emplace_back("北京");
        leftData.emplace_back("长沙");
        leftData.emplace_back("兰州");
        leftData.emplace_back("南京");
        leftData.emplace_back("上海");
        leftData.emplace_back("深圳");
        leftData.emplace_back("武汉");
        leftData.emplace_back("西安");
    }

    void updateList() {
        if (singleColumn) updateListSingleColumn();
        if (!singleColumn) updateListDoubleColumn();
    }

private:
    void updateListSingleColumn() {
        leftVerticalList1.setVisible(false);
        rightVerticalList1.setVisible(false);

        leftVerticalList0.clear();
        for (const auto city : leftData)
            leftVerticalList0.addSimpleLabel(city);
        rightVerticalList0.clear();
        for (const auto city : rightData)
            rightVerticalList0.addSimpleLabel(city);
    }

    void updateListDoubleColumn() {
        leftVerticalList1.setVisible(true);
        rightVerticalList1.setVisible(true);

        leftVerticalList0.clear();
        leftVerticalList1.clear();
        for (bool left = false; const auto city : leftData) {
            if (left) leftVerticalList1.addSimpleLabel(city);
            else leftVerticalList0.addSimpleLabel(city);
            left = !left;
        }
        rightVerticalList0.clear();
        rightVerticalList1.clear();
        for (bool left = false; const auto city : rightData) {
            if (left) rightVerticalList1.addSimpleLabel(city);
            else rightVerticalList0.addSimpleLabel(city);
            left = !left;
        }
    }
};

Window::Window()
    : MainWindow()
    , pimpl_(new Impl) {
    setFont(QFont { "monospace", 12 });

    auto h0 = new QHBoxLayout;
    h0->setAlignment(Qt::AlignCenter);
    h0->addWidget(&pimpl_->switchTheme);
    h0->addWidget(&pimpl_->switchThemeLabel);
    auto h1 = new QHBoxLayout;
    h1->setAlignment(Qt::AlignCenter);
    h1->addWidget(&pimpl_->switchShowMethod);
    h1->addWidget(&pimpl_->switchShowMethodLabel);

    auto v1 = new QVBoxLayout;
    v1->addLayout(h0);
    v1->addLayout(h1);

    auto g0 = new QGridLayout;
    g0->addWidget(&pimpl_->leftVerticalList0, 0, 0, 4, 1);
    g0->addWidget(&pimpl_->leftVerticalList1, 0, 1, 4, 1);
    g0->addWidget(&pimpl_->rightVerticalList0, 0, 3, 4, 1);
    g0->addWidget(&pimpl_->rightVerticalList1, 0, 4, 4, 1);
    g0->addWidget(&pimpl_->singleLeft2Right, 0, 2, 1, 1);
    g0->addWidget(&pimpl_->allLeft2Right, 1, 2, 1, 1);
    g0->addWidget(&pimpl_->singleRight2Left, 2, 2, 1, 1);
    g0->addWidget(&pimpl_->allRight2Left, 3, 2, 1, 1);

    auto v0 = new QVBoxLayout;
    v0->addLayout(v1);
    v0->addLayout(g0);

    pimpl_->switchShowMethodLabel.setText("切换显示方法");
    pimpl_->switchShowMethodLabel.setFont(font());
    pimpl_->switchThemeLabel.setText("切换主题颜色");
    pimpl_->switchThemeLabel.setFont(font());

    pimpl_->switchTheme.setFixedSize({ 60, 30 });
    pimpl_->switchShowMethod.setFixedSize({ 60, 30 });

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

    connect(&pimpl_->leftVerticalList0, &ListWidget::clicked,
        [this] { pimpl_->leftVerticalList1.clearSelection(); });
    connect(&pimpl_->leftVerticalList1, &ListWidget::clicked,
        [this] { pimpl_->leftVerticalList0.clearSelection(); });

    connect(&pimpl_->rightVerticalList0, &ListWidget::clicked,
        [this] { pimpl_->rightVerticalList1.clearSelection(); });
    connect(&pimpl_->rightVerticalList1, &ListWidget::clicked,
        [this] { pimpl_->rightVerticalList0.clearSelection(); });

    /// @todo 等列表完善了记得修一下这一坨石山
    connect(&pimpl_->singleLeft2Right, &PushButton::released, [this] {
        const auto selected0 = pimpl_->leftVerticalList0.currentIndex().row();
        const auto selected1 = pimpl_->leftVerticalList1.currentIndex().row();
        if (selected0 == -1 && selected1 == -1) return;
        const auto& focusList
            = selected0 == -1 ? pimpl_->leftVerticalList1 : pimpl_->leftVerticalList0;
        const auto item = focusList.currentItem()->text();
        const auto index = pimpl_->singleColumn
            ? selected0
            : (selected0 == -1 ? 2 * (selected1 + 1) - 1 : 2 * selected0);
        pimpl_->leftData.erase(pimpl_->leftData.begin() + index);
        pimpl_->rightData.push_back(item);
        pimpl_->updateList();
    });
    connect(&pimpl_->singleRight2Left, &PushButton::released, [this] {
        const auto selected0 = pimpl_->rightVerticalList0.currentIndex().row();
        const auto selected1 = pimpl_->rightVerticalList1.currentIndex().row();
        if (selected0 == -1 && selected1 == -1) return;
        const auto& focusList
            = selected0 == -1 ? pimpl_->rightVerticalList1 : pimpl_->rightVerticalList0;
        const auto item = focusList.currentItem()->text();
        const auto index = pimpl_->singleColumn
            ? selected0
            : (selected0 == -1 ? 2 * (selected1 + 1) - 1 : 2 * selected0);
        pimpl_->rightData.erase(pimpl_->rightData.begin() + index);
        pimpl_->leftData.push_back(item);
        pimpl_->updateList();
    });
    connect(&pimpl_->allLeft2Right, &PushButton::released, [this] {
        pimpl_->rightData.insert(
            pimpl_->rightData.end(), pimpl_->leftData.begin(), pimpl_->leftData.end());
        pimpl_->leftData.clear();
        pimpl_->updateList();
    });
    connect(&pimpl_->allRight2Left, &PushButton::released, [this] {
        pimpl_->leftData.insert(
            pimpl_->leftData.end(), pimpl_->rightData.begin(), pimpl_->rightData.end());
        pimpl_->rightData.clear();
        pimpl_->updateList();
    });
    connect(&pimpl_->switchShowMethod, &ConcaveSwitchButton::released, [this] {
        pimpl_->singleColumn = !pimpl_->singleColumn;
        pimpl_->updateList();
    });

    auto main = new QWidget;
    main->setLayout(v0);
    setCentralWidget(main);

    pimpl_->generateExamples();
    pimpl_->updateList();
}

Window::~Window() { delete pimpl_; }