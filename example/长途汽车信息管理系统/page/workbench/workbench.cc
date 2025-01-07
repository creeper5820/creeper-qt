#include "workbench.hh"
#include "operator.hh"
#include "top-area.hh"
#include "view.hh"

#include <creeper-qt/setting/color.hh>

using namespace creeper;

struct Workbench::Impl { };

Workbench::Workbench(QWidget* parent)
    : Extension(parent)
    , pimpl_(new Impl) {
    auto topLeftArea = new TopLeftArea;
    auto topCenterArea = new TopCenterArea;
    auto topRightArea = new TopRightArea;

    auto operatorArea = new OperatorArea;
    auto viewArea = new ViewArea;

    auto topAreaLayout = new QHBoxLayout;
    topAreaLayout->addWidget(topLeftArea);
    topAreaLayout->addWidget(topCenterArea);
    topAreaLayout->addWidget(topRightArea);

    auto mainViewLayout = new QHBoxLayout;
    mainViewLayout->addWidget(operatorArea, 0);
    mainViewLayout->addWidget(viewArea, 1);

    auto mainView = new RoundedRectangle;
    mainView->setLayout(mainViewLayout);
    mainView->setBackground(color::grey100);
    mainView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto verticalMainLayout = new QVBoxLayout;
    verticalMainLayout->setAlignment(Qt::AlignTop);
    verticalMainLayout->setSpacing(5);
    verticalMainLayout->setMargin(10);
    verticalMainLayout->addLayout(topAreaLayout);
    verticalMainLayout->addWidget(mainView);

    setLayout(verticalMainLayout);

    connect(operatorArea, &OperatorArea::changeView,
        [viewArea](std::size_t index) { viewArea->changeView(index); });
}

Workbench::~Workbench() { delete pimpl_; }

void Workbench::reloadTheme() { std::printf("Workbench reload theme\n"); }