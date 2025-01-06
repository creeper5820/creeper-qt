#include "workbench.hh"
#include "operator.hh"
#include "top-area.hh"

struct Workbench::Impl { };

Workbench::Workbench(QWidget* parent)
    : Extension(parent)
    , pimpl_(new Impl) {
    auto topArea = new TopArea;
    auto operatorArea = new OperatorArea;

    auto horizonMain = new QHBoxLayout;
    horizonMain->setAlignment(Qt::AlignTop);
    horizonMain->setSpacing(0);
    horizonMain->setContentsMargins(0, 0, 0, 0);
    horizonMain->addWidget(topArea);

    setLayout(horizonMain);
}

Workbench::~Workbench() { delete pimpl_; }

void Workbench::reloadTheme() { std::printf("Workbench reload theme\n"); }